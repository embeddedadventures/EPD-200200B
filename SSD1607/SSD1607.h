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

//EPD-200200B ePaper Display Arduino library
//Written originally by Embedded Adventures

#ifndef	__SSD1607_h
#define	__SSD1607_h

#include "config.h"
#include "utils.h"
#include "SPI.h"
#include "draw.h"
#include "draw_fonts.h"
#include "ea_bitmaps.h"

#define		F_SSD1607	1000000UL
#define		EPD_HT	200
#define		EPD_WD	200	

class SSD1607 {
private:
	uns8	_cs;
	uns8	_dc;
	uns8	_busy;
	uns8	_rst;
	uns8	_data;
	uns8	_clk;
	bool	_inverted;
	bool	_spiEmulated;
	
	void	write_command(uns8 cmd);
	void	write_command(uns8 cmd, uns8 param);
	void	write_command(uns8* dataPtr, uns8 dataLen);
	
public:
	SSD1607(uns8 cs, uns8 dc, uns8 busy, uns8 rst);
	SSD1607(uns8 data, uns8 clk, uns8 cs, uns8 dc, uns8 busy, uns8 rst);
	void	init();
	void	power_on();
	void	invert(bool en);
	void	set_ram_area(uns8 xStart, uns8 xEnd, uns8 yStart, uns8 yStart1, uns8 yEnd, uns8 yEnd1);
	void 	set_ram_ptr(uns8 xAddr, uns8 yAddr, uns8 yAddr1);
	void	displayFullRev(uns8* frameBuffer);
	void	write_ram_rev(uns8 xSize, uns32 ySize, uns8* frameBuffer);
};

#endif