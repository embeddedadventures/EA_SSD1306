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

#include "EA_SSD1306.h"

//#define SSD1306_DEBUG

#ifdef	SSD1306_DEBUG
	#define	DEBUG_CMD2(fn, param0, param1)	Serial.print(fn, HEX); Serial.print(": "); Serial.print(param0, HEX); Serial.print(", "); Serial.println(param1, HEX);
	#define	DEBUG_CMD1(fn, param)			Serial.print(fn, HEX); Serial.print(": "); Serial.println(param, HEX);
	#define DEBUG_CMD0(fn)					Serial.println(fn, HEX);
	#define DEBUG_STATS()					Serial.println("Display Stats\n--------"); Serial.print(OLED_WIDTH, DEC); Serial.print("x"); Serial.println(OLED_HEIGHT, DEC); Serial.print("frame "); Serial.println(_frame_size, DEC);
#else
	#define	DEBUG_CMD2(fn, param0, param1)
	#define DEBUG_CMD1(fn, param) 
	#define DEBUG_CMD0(fn)
	#define DEBUG_STATS()
#endif

SSD1306::SSD1306() {
	_height = OLED_HEIGHT;
	_width = OLED_WIDTH;
	_mem_addressing_mode = PAGE_ADDR_MODE;
}

SSD1306::SSD1306(uns16 height, uns16 width) {
	if (height > MAX_HEIGHT)
		_height = MAX_HEIGHT;
	else
		_height = height;
	if (width > MAX_WIDTH)
		_width = MAX_WIDTH;
	else
		_width = width;
	_mem_addressing_mode = PAGE_ADDR_MODE;
}

void SSD1306::command(uns8 cmd) {
	Wire.beginTransmission(SSD1306_ADDR);
	Wire.write(0x00);
	Wire.write(cmd);
	Wire.endTransmission();
}

void SSD1306::set_clk_div_ratio(uns8 div_ratio, uns8 fosc) {
	div_ratio &= 0x0F;
	fosc = (fosc & 0x0F) << 4;
	command(CMD_CLKDIV_RATIO);
	command(fosc | div_ratio);
	DEBUG_CMD1(CMD_CLKDIV_RATIO, (fosc | div_ratio));
}

void SSD1306::set_memory_addressing(uns8 mode) {
	_mem_addressing_mode = mode & 0x03;
	command(CMD_MEM_ADDR_MODE);
	command(_mem_addressing_mode);
	DEBUG_CMD1(CMD_MEM_ADDR_MODE, _mem_addressing_mode);
}

void SSD1306::set_display_offset(uns8 offset) {
	command(CMD_DISP_OFFSET);
	command(offset & 0x3F);
	DEBUG_CMD1(CMD_DISP_OFFSET, offset & 0x3F);
}

void SSD1306::set_mux_ratio(uns8 mux) {
	mux &= 0x3F;
	//If invalid entry, set to default
	if (mux < 15)
		mux = 0x3F;
	command(CMD_MUX_RATIO);
	command(mux);
	DEBUG_CMD1(CMD_MUX_RATIO, mux);
}

void SSD1306::remap_segment(bool remap) {
	if (remap) {
		command(CMD_SEGMENT_REMAP | 0x01);
		DEBUG_CMD0(CMD_SEGMENT_REMAP | 0x01);
	}
	else {
		command(CMD_SEGMENT_REMAP);
		DEBUG_CMD0(CMD_SEGMENT_REMAP);
	}
	
}

void SSD1306::set_com_pins(bool sequential, bool remap_lr) {
	uns8 comPins = 0x02;
	if (!sequential)
		comPins |= 0x10;
	if (remap_lr)
		comPins |= 0x20;
	command(CMD_COM_PINS);
	command(comPins);
	DEBUG_CMD1(CMD_COM_PINS, comPins);
}

void SSD1306::set_com_scan_up(bool scanUp) {
	if (scanUp) {
		command(CMD_COMSCAN_UP);
		DEBUG_CMD0(CMD_COMSCAN_UP);
	}
	else {
		command(CMD_COMSCAN_DOWN);
		DEBUG_CMD0(CMD_COMSCAN_DOWN);
	}
	
}

void SSD1306::set_precharge(uns8 phase1, uns8 phase2) {
	phase1 &= 0x0F;
	phase2 = (phase2 &0x0F) << 4;
	command(CMD_PRECHARGE_PD);
	command(phase1 | phase2);
	DEBUG_CMD1(CMD_PRECHARGE_PD, phase1 | phase2);
}

void SSD1306::set_vcom_deselect(uns8 vcom) {
	vcom &= 0x07;
	if (vcom == 0x01)
		vcom = 0x02;
	vcom = vcom << 5;
	command(CMD_VCOM_DESELECT);
	command(vcom & 0x70);
	DEBUG_CMD1(CMD_VCOM_DESELECT, vcom & 0x70);
}

void SSD1306::enable_charge_pump() {
	command(CMD_CHARGEPUMP_EN);
	command(0x14);
	DEBUG_CMD1(CMD_CHARGEPUMP_EN, 0x14);
}

void SSD1306::set_column_address(uns8 startAddr, uns8 endAddr) {
	command(CMD_SET_COL_ADDR);
	command(startAddr & 0x7F);
	command(endAddr & 0x7F);
	DEBUG_CMD2(CMD_SET_COL_ADDR, startAddr & 0x7F, endAddr & 0x7F);
}

void SSD1306::set_page_address(uns8 startAddr, uns8 endAddr) {
	//Page addressing mode
	if (_mem_addressing_mode == PAGE_ADDR_MODE) {
		command(0xB0 | (startAddr & 0x03));
		DEBUG_CMD0(0xB0 | (startAddr & 0x03));
	}
	//Horizontal or vertical addressing mode
	else {
		command(CMD_SET_PAGE_ADDR);
		command(startAddr & 0x03);
		command(endAddr & 0x03);
		DEBUG_CMD2(CMD_SET_PAGE_ADDR, startAddr & 0x03, endAddr & 0x03);
	}
	
}

void SSD1306::paint(uns8 *buffer) {
	set_column_address(OLED_OFFSET, (OLED_WIDTH + OLED_OFFSET) - 1);
	set_page_address(0, (OLED_HEIGHT / PAGE_HEIGHT) - 1);
	for (int i = 0; i < _frame_size; i++) {
		Wire.beginTransmission(SSD1306_ADDR);
		Wire.write(CMD_STARTLINE);
		for (int j = 0; j < PAGE_HEIGHT; j++) {
			Wire.write(buffer[i]);
			i++;
		}
		i--;
		Wire.endTransmission();
	}
}

void SSD1306::clear() {
	set_column_address(0, MAX_WIDTH - 1);
	set_page_address(0, NUM_PAGES - 1);
	Serial.println("about to start, clear");
	for (int i = 0; i < MAX_WIDTH; i++) {
		Wire.beginTransmission(SSD1306_ADDR);
		Wire.write(CMD_STARTLINE);
		for (int j = 0; j < PAGE_HEIGHT; j++) {
			Wire.write(0x00);
		}
		Wire.endTransmission();
	}
}

void SSD1306::invert(bool invert) {
	if (invert) {
		command(CMD_DISPLAY_INVERT);
		DEBUG_CMD0(CMD_DISPLAY_INVERT);
	}
	else {
		command(CMD_DISPLAY_NORMAL);
		DEBUG_CMD0(CMD_DISPLAY_NORMAL);
	}
}

void SSD1306::setContrast(uns8 contrast) {
	command(CMD_CONTRAST);
	command(contrast);
	DEBUG_CMD1(CMD_CONTRAST, contrast);
}

void SSD1306::setStartLine(uns8 start) {
	command(CMD_STARTLINE | (start & 0x3F));
	DEBUG_CMD0(CMD_STARTLINE | (start & 0x3F));
}

void SSD1306::begin() {
	_frame_size = OLED_HEIGHT * OLED_WIDTH / 8;
	DEBUG_STATS();
	
	set_mux_ratio(OLED_HEIGHT - 1);
	set_display_offset(0);
	setStartLine(0);
	remap_segment(false);
	set_memory_addressing(VERT_ADDR_MODE);
	set_com_scan_up(true);
	set_com_pins(false, false);	//Non-sequential pin COM, don't remap left/right
	setContrast(0xCF);
	command(CMD_DISPLAY_OFF);
	invert(false);
	set_clk_div_ratio(0, 8);	//CLKDIV = 1, FOSC = 8
	set_precharge(1, 15);		//Phase 1 = 1 dclk, Phase 2 = 15 dclks
	set_vcom_deselect(0x02);	//0.77 * Vcc
	enable_charge_pump();
	clear();
	command(CMD_DISPLAY_WHITE);
	command(CMD_DISPLAY_ON);
}



