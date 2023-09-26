// simple project using Arduino UNO and 128x128 SD1107 OLED Display to show static image of compass 
// created by upir, 2023
// youtube channel: https://www.youtube.com/upir_upir

// YOUTUBE VIDEO: https://youtu.be/G1C09eoJ07c
// YOUTUBE VIDEO CLOCK: https://youtu.be/srgsBWHSNSQ
// WOKWI sketch: https://wokwi.com/projects/376931330900285441

// links from the video:
// 128x128 SH1107 OLED Display: https://s.click.aliexpress.com/e/_DdOCQHj
// 128x64 SSD1306 OLED Display: https://s.click.aliexpress.com/e/_DCKdvnh
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_ApbCwx
// Photopea (online Photoshop-like tool): https://www.photopea.com/
// u8g2 documentation: https://github.com/olikraus/u8g2/wiki/u8gvsu8g2
// Image2cpp (convert array to image): https://javl.github.io/image2cpp/


// Related videos:
// Arduino OLED Clock Project: https://youtu.be/srgsBWHSNSQ
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
#include "compass_image.h" // image generated using the image2cpp tool is stored in a separate header file

// if you see the image offset on the display, try uncommenting the "PIMORONI" version of the display, see video for more details
//U8G2_SH1107_128X128_1_HW_I2C u8g2(U8G2_R0); // final display, 128x128px [page buffer, size = 128 bytes], HW IIC connection
U8G2_SH1107_128X128_2_HW_I2C u8g2(U8G2_R0); // final display, 128x128px [page buffer, size = 256 bytes], HW IIC connection
//U8G2_SH1107_PIMORONI_128X128_2_HW_I2C u8g2(U8G2_R0); //[page buffer, size = 256 bytes], HW IIC connection

// IIC connection of the OLED display and Arduino UNO
// +5V > +5V
// GND > GND
// SCL (serial clock) > A5 or SCL
// SDA (serial data) > A4 or SDA


void setup(void) { // Arduino setup
  u8g2.begin();  // begin the u8g2 library
  u8g2.setContrast(255); // set display contrast/brightness
}

void loop(void) { // main Arduino loop

  u8g2.firstPage(); // select the first page of the display (page is 128x8px or 128x16px), since we are using the page drawing method of the u8g2 library
  do { // draw

    u8g2.drawXBMP(0, 0, 128, 128, epd_bitmap_oled_compass_128x128px); // draw fullscreen 128x128px image (drawn in Photopea, generated using image2cpp)

  } while ( u8g2.nextPage() ); // go over all the pages until the whole display is updated
}
