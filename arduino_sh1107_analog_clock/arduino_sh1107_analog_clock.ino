// simple project using Arduino UNO and 128x64 OLED Display to display analog clock
// note - this project is meant for the 128x128 SH1107 OLED display, but WOKWI currently only supports 128x64 OLED display
// for this reason, the preview here only shows half of the design
// created by upir, 2023
// youtube channel: https://www.youtube.com/upir_upir

// YOUTUBE VIDEO: https://youtu.be/srgsBWHSNSQ

// links from the video:
// 128x128 SH1107 OLED Display: https://s.click.aliexpress.com/e/_DdOCQHj
// 128x64 SSD1306 OLED Display: https://s.click.aliexpress.com/e/_DCKdvnh
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_ApbCwx
// Photopea (online Photoshop-like tool): https://www.photopea.com/
// u8g2 documentation: https://github.com/olikraus/u8g2/wiki/u8gvsu8g2


// Related videos:
// Arduino Parking Sensor - https://youtu.be/sEWw087KOj0
// Turbo pressure gauge with Arduino and OLED display - https://youtu.be/JXmw1xOlBdk
// Arduino Car Cluster with OLED Display - https://youtu.be/El5SJelwV_0
// Knob over OLED Display - https://youtu.be/SmbcNx7tbX8
// Arduino + OLED = 3D ? - https://youtu.be/kBAcaA7NAlA
// Arduino OLED Gauge - https://youtu.be/xI6dXTA02UQ
// Smaller & Faster Arduino - https://youtu.be/4GfPQoIRqW8


#include <Arduino.h>
#include <U8g2lib.h> // u8g2 library for drawing on OLED display - needs to be installed in Arduino IDE first
#include <Wire.h> // wire library for IIC communication with the OLED display

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0); // set the OLED display to 128x64px, HW IIC, no rotation, used in WOKWI
//U8G2_SH1107_128X128_1_HW_I2C u8g2(U8G2_R0); // final display, 128x128px [page buffer, size = 128 bytes], HW IIC connection

// IIC connection of the OLED display and Arduino UNO
// +5V > +5V
// GND > GND
// SCL (serial clock) > A5 or SCL
// SDA (serial data) > A4 or SDA

const unsigned char upir_logo [] PROGMEM = {   // upir logo 
  0xEA, 0x3A, 0xAA, 0x28, 0x6A, 0x1A, 0x26, 0x2A, };

int center_x = 64; // display x center, 64px for the 128x128px display
int center_y = 64; // display y center, 64px for the 128x128px display

// hardcoded time values
int time_minutes = 10;
int time_hours = 10;
int time_seconds = 45;

void setup(void) { // Arduino setup
  u8g2.begin();  // begin the u8g2 library
  u8g2.setContrast(255); // set display contrast/brightness
}

// draw the background - fullscreen circle, dots for seconds, big tickmarks, numbers
void draw_background() {

  float xpos;
  float ypos;
  float xpos2;
  float ypos2;  

  u8g2.setDrawColor(1); // set the drawing color to white    
  u8g2.drawCircle(center_x, center_y, 63, U8G2_DRAW_ALL); // draw fullscreen circle

  // draw 60 dots (pixels) around the circle, one for every minute/second
  for (int i=0; i<60; i++) { // draw 60 pixels around the circle
    xpos = round(center_x + sin(radians(i * 6)) * 60); // calculate x pos based on angle and radius
    ypos = round(center_y - cos(radians(i * 6)) * 60); // calculate y pos based on angle and radius
    
    u8g2.drawPixel(xpos,ypos); // draw white pixel on position xpos and ypos
  }

  // drawing big tickmarks
  for (int i=0; i<12; i++) {
    if((i % 3) != 0) { // only draw tickmarks for some numbers, leave empty space for 12, 3, 6, and 9
      xpos = round(center_x + sin(radians(i * 30)) * 54); // calculate x pos based on angle and radius
      ypos = round(center_y - cos(radians(i * 30)) * 54); // calculate y pos based on angle and radius
      xpos2 = round(center_x + sin(radians(i * 30)) * 46); // calculate x pos based on angle and radius
      ypos2 = round(center_y - cos(radians(i * 30)) * 46); // calculate y pos based on angle and radius      
      
      u8g2.drawLine(xpos, ypos, xpos2, ypos2); // draw a line for a tickmark
    }
  }

  // draw labels 12, 3, 6 and 9, positions are hardcoded
  u8g2.setFont(u8g2_font_8x13B_mn); // set the u8g2 font
  u8g2.drawStr(57,20,"12");
  u8g2.drawStr(112,69,"3");
  u8g2.drawStr(61,120,"6");
  u8g2.drawStr(9,69,"9");            
}

// draw thin hand = second hand
void draw_hand_thin (int hand_angle, int hand_lenght_long, int hand_legth_short) {

  float xpos;
  float ypos;
  float xpos2;
  float ypos2;  

  // calculate starting and ending position of the second hand
  xpos = round(center_x + sin(radians(hand_angle)) * hand_lenght_long); // calculate x pos based on angle and radius
  ypos = round(center_y - cos(radians(hand_angle)) * hand_lenght_long); // calculate y pos based on angle and radius
  xpos2 = round(center_x + sin(radians(hand_angle + 180)) * hand_legth_short); // calculate x pos based on angle and radius
  ypos2 = round(center_y - cos(radians(hand_angle + 180)) * hand_legth_short); // calculate y pos based on angle and radius  

  u8g2.drawLine(xpos, ypos, xpos2, ypos2); // draw the main line
  u8g2.setDrawColor(0); // black color
  u8g2.drawDisc(xpos2, ypos2, 3); // draw small filled black circle
  u8g2.setDrawColor(1); // white color
  u8g2.drawCircle(xpos2, ypos2, 3, U8G2_DRAW_ALL); // draw small outline white circle
}


// draw bold hand = minute hand and hour hand
void draw_hand_bold (int hand_angle, int hand_lenght_long, int hand_legth_short, int hand_dot_size) {

  float xpos;
  float ypos;
  float xpos2;
  float ypos2;  

  float tri_xoff;
  float tri_yoff;  

  // calculate positions of the two circles
  xpos = round(center_x + sin(radians(hand_angle)) * hand_lenght_long); // calculate x pos based on angle and radius
  ypos = round(center_y - cos(radians(hand_angle)) * hand_lenght_long); // calculate y pos based on angle and radius
  xpos2 = round(center_x + sin(radians(hand_angle)) * hand_legth_short); // calculate x pos based on angle and radius
  ypos2 = round(center_y - cos(radians(hand_angle)) * hand_legth_short); // calculate y pos based on angle and radius  

  tri_xoff = round( sin(radians(hand_angle + 90)) * hand_dot_size);
  tri_yoff = round(-cos(radians(hand_angle + 90)) * hand_dot_size);  

  u8g2.drawLine(center_x, center_y, xpos2, ypos2); // draw the line from one circle to the center
  u8g2.drawDisc(xpos, ypos, hand_dot_size); // draw filled white circle
  u8g2.drawDisc(xpos2, ypos2, hand_dot_size); // draw filled white circle

  // two filled triangles are used to draw a rotated rectangle between two circles
  u8g2.drawTriangle(xpos + tri_xoff, ypos + tri_yoff,
                    xpos - tri_xoff, ypos - tri_yoff,
                    xpos2 + tri_xoff, ypos2 + tri_yoff);

  u8g2.drawTriangle(xpos2 + tri_xoff, ypos2 + tri_yoff,
                    xpos2 - tri_xoff, ypos2 - tri_yoff,
                    xpos - tri_xoff, ypos - tri_yoff);


}



void loop(void) { // main Arduino loop

  // increment the time
  time_seconds++;
  if (time_seconds >= 60) {
    time_seconds = 0;
    time_minutes++;
    if (time_minutes >= 60) {
      time_minutes = 0;
      time_hours++;
      if (time_hours >= 12) {
        time_hours = 0;
      }
    }
  }


  u8g2.firstPage(); // select the first page of the display (page is 128x8px), since we are using the page drawing method of the u8g2 library
  do { // draw

    draw_background(); // draw the background - fullscreen circle, dots for seconds, big tickmarks, numbers

    // draw the needles with angles based on the time value
    draw_hand_bold(time_minutes*6, 48, 15, 2); // minute hand 
    draw_hand_bold(time_hours*30 + (time_minutes / 2), 32, 15, 2); // hour hand
    draw_hand_thin(time_seconds*6, 56, 24); // second hand

    u8g2.drawXBMP(57, 24, 16, 4, upir_logo); // draw upir logo
    
    // draw the center circle to cover the center part of the hands
    u8g2.setColorIndex(0); // black color
    u8g2.drawDisc(center_x, center_y, 4);
    u8g2.setColorIndex(1); // white color
    u8g2.drawCircle(center_x, center_y, 4);   

  } while ( u8g2.nextPage() ); // go over all the pages until the whole display is updated
}
