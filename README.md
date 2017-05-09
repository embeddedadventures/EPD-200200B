# SSD1607 #
Arduino library for the SSD1607 e-Paper display driver <a href="http://www.embeddedadventures.com/epaper_display_EPD-200200B.html">EPD-200200B</a>.

**IMPORTANT** 

This module runs on a 3.3V power supply and 3.3V logic. If using a 5V MCU, place a level translator in between the logic connections to bring the logic levels down to 3.3V. For example, our <a href="http://www.embeddedadventures.com/level_translator_module_mod-1003B.html">Bi-Directional Level Translator</a> would be a good solution (you'll need two since 6 pins are used for communication).

## Using the library ##

The driver requires the <a href="https://github.com/embeddedadventures/draw">Embedded Adventures graphics library</a> in order to display text and shapes. 

The Arduino sample sketches show the minimum functions necessary to display some text on the ePaper display. In addition to initializing the graphics library (draw and draw_font), the SSD1607 library needs to be initialized by calling the two functions

	invert(true);
	init();

The graphics library (draw) allows for usgae with multiple display drivers by allowing the user to define the main display function:

	drv_paint();

This function must be defined in the Arduino sketch. 

## Tested with the following boards ##
- <a href="http://www.embeddedadventures.com/arduno_uno_PLT-AUNO.html">Arduino Uno</a>/<a href="http://www.embeddedadventures.com/arduno_uno_plt-auno3-cm.html">compatible</a>
	- 11   - DATA
	- 13   - CLK
- ESP32 Module
	- IO23 - DATA
	- IO18 - CLK
- ESP8266 Module (<a href="http://www.embeddedadventures.com/esp8266_wifi_module_wrl-esp7.html">ESP7</a> and <a href="http://www.embeddedadventures.com/esp8266_wifi_module_wrl-esp12e.html">ESP12</a>)
	- IO13 - DATA
	- IO14 - CLK
- SAMD21-based Arduino board