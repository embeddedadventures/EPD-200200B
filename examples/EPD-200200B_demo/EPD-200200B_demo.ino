/*
Copyright (c) 2017, Embedded Adventures, www.embeddedadventures.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.

Contact us at admin [at] embeddedadventures.com
*/

//Sample sketch for the EPD-200200B ePaper Display

#include <SPI.h>
#include <draw.h>
#include <SSD1607.h>


#if defined(ESP32)
  #define DCPIN   12
  #define CSPIN   13
  #define BUSYPIN 14
  #define RSTPIN  25
#elif defined(ESP8266)
  #define DCPIN   12
  #define CSPIN   2
  #define BUSYPIN 5
  #define RSTPIN  15
#else
  #define DCPIN   7
  #define CSPIN   6
  #define BUSYPIN 5
  #define RSTPIN  4
#endif

/* Hardware SPI pins are used for the ESP32/ESP8266/ATmega/ATtiny based boards
 *  ESP8266
 *    MOSI - IO13
 *    SCK  - IO14
 *  ESP32
 *    MOSI - IO23
 *    SCK  - IO18
 *  ATmega/ATtiny
 *    MOSI - 11
 *    SCK  - 13
 */
SSD1607 epd(CSPIN, DCPIN, BUSYPIN, RSTPIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Welcome to the Embedded Adventures demo sketch for the EPD-200200B");
  
  draw_init(EPD_HT, EPD_WD, 1);
  draw_fonts_init();
  
  epd.invert(true);
  epd.init();
  drawLogo(70, 175);
}

void loop() {

}

//This function is necessary for using the ePaper Display
void drv_paint() {
  epd.displayFullRev(draw_buffer);
}

void drawLogo(draw_x_type x, draw_y_type y) {
  draw_clear_screen();
  delay(100);
  draw_bitmap(x, y-60, 1, embedded_bitmap);
  draw_bitmap(x, y-75, 1, adventures_bitmap);
  draw_bitmap(x, y, 1, e_big_bitmap);
  draw_bitmap(x+30, y, 1, a_big_bitmap);
  draw_fonts_print_str(DRAW_FONT_10DOUBLE_ID, x-10, y - 100, 128, 0, 2, "EPD-200200B");
  draw_fonts_print_str(DRAW_FONT_10NORMAL_ID, x-30, y - 115, 128, 0, 2, "200X200 EPAPER DISPLAY");
  draw_paint();
}

