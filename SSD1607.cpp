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

#include "SSD1607.h"

#ifdef	EPD200200B
	uns8 LUT_full[31] = {
	0x32,	// command
/*	0x11,0x11,0x10,0x02,	// full
	0x02,0x22,0x22,0x22,
	0x22,0x22,0x51,0x51,
	0x55,0x88,0x08,0x08,
    0x88,0x88,0x00,0x00,
	0x34,0x23,0x12,0x21,
	0x24,0x28,0x22,0x21,
	0xA1,0x01
*/
 0x02 //C221 25C Full update waveform
,0x02
,0x01
,0x11
,0x12
,0x12
,0x22
,0x22
,0x66
,0x69
,0x69
,0x59
,0x58
,0x99
,0x99
,0x88
,0x00
,0x00
,0x00
,0x00
,0xF8
,0xB4
,0x13
,0x51
,0x35
,0x51
,0x51
,0x19
,0x01
,0x00
	};
	uns8 GDOControl[] = {
		0x01,				//Driver Output Control Command
		(EPD_HT - 1) % 256,	(EPD_HT - 1) / 256,	//MUX Setting - A[8:0] + 1
		0x00
	};
	uns8 BoosterSoftStart[] = {
		0x0C,			//Booster Enable Command
		0xD7,			//Phase1 = A[7] + A[6:0], default 0x87
		0xD6,			//Phase2 = B[7] + B[6:0], default 0x86
		0x9D			//Phase3 = C[7] + C[6:0]
	};
	uns8 RAMBypass[] = {
		0x21,					//Display Update Control 1
		0x8F					//Enable bypass
	};
	uns8 MasterActivation[] = {
		0x22,				//Display Update Control 2
		0xF0
	};
	uns8 GDVoltage[] = {0x03, 0x00};	//Gate voltage
	uns8 SDVoltage[] = {0x04, 0x0A};	//Source voltage 
	uns8 VCOMVoltage[] = {0x2C, 0xA8};
	uns8 BOOSTERFB[] = {0xf0,0x1f};	// Source voltage +15V/-15V
	uns8 DummyLine[] = {0x3a,0x1a};	// 4 dummy line per gate
	uns8 GateTime[] = {0x3b,0x08};	// 2us per line
	uns8 BorderWaveform[] = {0x3c,0x33};	// Border
	uns8 RamDataEntryMode[] = {0x11,0x01};	// Ram data entry mode
#else
	#error "A display must be defined!"
#endif

SSD1607::SSD1607(uns8 cs, uns8 dc, uns8 busy, uns8 rst) {
	_cs = cs;
	_dc = dc;
	_busy = busy;
	_rst = rst;
	_inverted = false;
	
	SPI.begin();
	SPI.beginTransaction(SPISettings(F_SSD1607, MSBFIRST, SPI_MODE2));
	
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	pinMode(_busy, INPUT);
	pinMode(_rst, OUTPUT);
}

void SSD1607::write_command(uns8 cmd) {
	digitalWrite(_cs, LOW);
	digitalWrite(_dc, LOW);
	SPI.transfer(cmd);
	digitalWrite(_cs, HIGH);
}

void SSD1607::write_command(uns8 cmd, uns8 param) {
	while(digitalRead(_busy) == HIGH) {	}
	Serial.println("Not busy!");
	
	digitalWrite(_cs, LOW);
	digitalWrite(_dc, LOW);
	SPI.transfer(cmd);
	digitalWrite(_dc, HIGH);
	SPI.transfer(param);
	digitalWrite(_cs, HIGH);
	
}

void SSD1607::write_command(uns8* dataPtr, uns8 dataLen) {
	uns8 *temp;
	temp = dataPtr;
	
	digitalWrite(_cs, LOW);
	digitalWrite(_dc, LOW);
	SPI.transfer(*temp);
	temp++;
	digitalWrite(_dc, HIGH);
	
	for (int i = 1; i < dataLen; i++) {
		SPI.transfer(*temp);
		temp++;
	}
	
	digitalWrite(_cs, HIGH);
	
}

void SSD1607::init() {	
	digitalWrite(_rst, LOW);
	delay(100);
	digitalWrite(_rst, HIGH);
	delay(100);
	write_command(GDOControl, sizeof(GDOControl));
	write_command(BoosterSoftStart, sizeof(BoosterSoftStart));	// X decrease, Y decrease
	write_command(VCOMVoltage, sizeof(VCOMVoltage));		// VCOM setting
	write_command(DummyLine, sizeof(DummyLine));	// dummy line per gate
	write_command(GateTime, sizeof(GateTime));		// Gage time setting
	write_command(RamDataEntryMode, sizeof(RamDataEntryMode));	// X increase, Y decrease
	
	set_ram_area(0x00,(EPD_WD-1)/8,(EPD_HT-1)%256,(EPD_HT-1)/256,0x00,0x00);	// X-source area,Y-gage area
    set_ram_ptr(0x00,(EPD_HT-1)%256,(EPD_HT-1)/256);	// set ram
	write_command(&LUT_full[0], sizeof(LUT_full));
	power_on();
	delay(100);
	
	
	//draw_logo(70, 175);
}

void SSD1607::power_on() {
	write_command(0x22, 0xC0);
	write_command(0x20);
}

void SSD1607::invert(bool en) {
	if (en)
		_inverted = true;
	else
		_inverted = false;
}

void SSD1607::set_ram_area(uns8 xStart, uns8 xEnd, uns8 yStart, uns8 yStart1, uns8 yEnd, uns8 yEnd1) {
	uns8 ramAreaX[3] = {0x44, xStart, xEnd};
	uns8 ramAreaY[5] = {0x45, yStart, yStart1, yEnd, yEnd1};
	write_command(ramAreaX, sizeof(ramAreaX));
	write_command(ramAreaY, sizeof(ramAreaY));
	
}

void SSD1607::set_ram_ptr(uns8 xAddr, uns8 yAddr, uns8 yAddr1) {
	uns8 xPtr[2] = {0x4E, xAddr};
	uns8 yPtr[3] = {0x4F, yAddr, yAddr1};
	write_command(xPtr, sizeof(xPtr));
	write_command(yPtr, sizeof(yPtr));
}

void SSD1607::write_ram_rev(uns8 xSize, uns32 ySize, uns8* frameBuffer) {
	int i = 0,j = 0;
	static const unsigned char BitReverseTable256[256] = 
		{
		#   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
		#   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
		#   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
			R6(0), R6(2), R6(1), R6(3)
		};
	if(xSize%8 != 0) {
		xSize = xSize+(8-xSize%8);
	}
	xSize = xSize/8;

	//while(isEPD_W21_BUSY == 1); //wait
	//ReadBusy();   
	while(digitalRead(_busy) == HIGH) {	}
	Serial.println("Not busy!");

	digitalWrite(_cs, LOW);
	digitalWrite(_dc, LOW);

	SPI.transfer(0x24);

	//EPD_W21_DC_1;   //data write
	digitalWrite(_dc, HIGH);

	if (_inverted) {
	for(i=0;i<ySize;i++) {
	 for(j=0;j<xSize;j++) {
	  SPI.transfer(~BitReverseTable256[*frameBuffer]);
	  frameBuffer++;
	 }
	}
	} 
	else {  
	for(i=0;i<ySize;i++) {
	 for(j=0;j<xSize;j++) {
	  SPI.transfer(BitReverseTable256[*frameBuffer]);
	  frameBuffer++;
	 }
	}
	}  
	//EPD_W21_CS_1;
	digitalWrite(_cs, HIGH);
}

void SSD1607::displayFullRev(uns8* frameBuffer) {
	 set_ram_ptr(0x00,(EPD_HT-1)%256,(EPD_HT-1)/256);	// set ram
	 write_ram_rev(EPD_WD, EPD_HT, &frameBuffer[0]);
	 write_command(0x22, 0xC7);
	 write_command(0x20);
	 write_command(0xFF);
}


