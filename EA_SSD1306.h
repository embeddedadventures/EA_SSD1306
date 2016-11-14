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

#ifndef	_EA_SSD1306_h_
#define	_EA_SSD1306_h_

#include "Arduino.h"
#include "Wire.h"
#include "draw.h"
#include "draw_fonts.h"

#define SSD1306_DEBUG

#define		SSD1306_ADDR	0x3C
#define		MAX_HEIGHT		64
#define		MAX_WIDTH		128
#define		NUM_PAGES		8

#define		OLED_HEIGHT		32
#define		OLED_WIDTH		64
#define		OLED_OFFSET		32
#define		PAGE_HEIGHT		8

//SSD1306 Command Set
#define		CMD_DISPLAY_ON		0xAF
#define		CMD_DISPLAY_WHITE	0xA4
#define		CMD_DISPLAY_OFF		0xAE
#define		CMD_DISPLAY_INVERT	0xA7
#define		CMD_DISPLAY_NORMAL	0xA6
#define		CMD_CONTRAST		0x81
#define		CMD_CLKDIV_RATIO	0xD5
#define		CMD_MUX_RATIO		0xA8
#define		CMD_DISP_OFFSET		0xD3
#define		CMD_STARTLINE		0x40
#define		CMD_CHARGEPUMP_EN	0x8D
#define		CMD_MEM_ADDR_MODE	0x20
#define		CMD_SEGMENT_REMAP	0xA0	
#define		CMD_COMSCAN_UP		0xC0
#define		CMD_COMSCAN_DOWN	0xC8
#define		CMD_COM_PINS		0xDA	//create COM pins settings
#define		CMD_PRECHARGE_PD	0xD9
#define		CMD_VCOM_DESELECT	0xDB	//Create mask
#define		CMD_SET_COL_ADDR	0x21
#define		CMD_SET_PAGE_ADDR	0x22

#define		HRZN_ADDR_MODE		0
#define		VERT_ADDR_MODE		1
#define		PAGE_ADDR_MODE		2


class SSD1306 {
private:
	uns16	_height;
	uns16	_width;
	uns8	_mem_addressing_mode; //0 = horizontal, 1 = vertical, 2 = page
	uns16	_frame_size;
	
	void	command(uns8 cmd);
	inline void		set_memory_addressing(uns8 mode = PAGE_ADDR_MODE);
	inline void		set_display_offset(uns8 offset = 0x00);
	inline void		set_clk_div_ratio(uns8 div_ratio = 0x00, uns8 fosc = 0x08);
	inline void		set_mux_ratio(uns8 mux = 0x3F);
	inline void		remap_segment(bool remap = false);
	inline void		set_com_pins(bool sequential = false, bool remap_lr = false);
	inline void		set_com_scan_up(bool scanUp = true);
	inline void		set_precharge(uns8 phase1 = 0x02, uns8 phase2 = 0x02);
	inline void		set_vcom_deselect(uns8 vcom = 0x02);
	inline void		enable_charge_pump();
	
	void	set_column_address(uns8 startAddr, uns8 endAddr);
	void	set_page_address(uns8 startAddr, uns8 endAddr = 0xFF);
	
public:
	SSD1306();
	SSD1306(uns16 height, uns16 width);
	
	void	begin();
	void	paint(uns8 *buffer);
	void	clear();
	void	invert(bool invert = false);
	void	setContrast(uns8 contrast = 0x7F);
	inline void		setStartLine(uns8 start);
};

#endif