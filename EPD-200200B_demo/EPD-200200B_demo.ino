/*
Copyright (c) 2016, Embedded Adventures, www.embeddedadventures.com
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

#include <SPI.h>
#include <draw.h>
#include <SSD1607.h>

const uns8 dcPin = 12;
const uns8 csPin = 2;
const uns8 busyPin = 5;
const uns8 rstPin = 15;

SSD1607 epd(csPin, dcPin, busyPin, rstPin);

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
  draw_fonts_print_str(DRAW_FONT_10NORMAL_ID, x-5, y - 100, 128, 0, 2, "EPD-200200B");
  draw_fonts_print_str(DRAW_FONT_10NORMAL_ID, x-30, y - 115, 128, 0, 2, "200X200 EPAPER DISPLAY");
  draw_paint();
}

