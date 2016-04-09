
/*******************************************************************************
 *
 *	VGA BIOS library
 *
 *	Portions of code from David Brackeen
 *	http://www.brackeen.com/home/vga/
 *	Portions of code from Brennan Underwood
 *	http://www.delorie.com/djgpp/doc/brennan/brennan_access_vga.html
 *	Additional information provided by Xavier Leclercq
 *	http://www.xaff.org/VGA/vga.html
 *	Additional information provided by Intel
 *	http://www.msc-ge.com/download/pc-system/ipc-produkte/pisa/VGA%20BIOS%20CT%2069030%20Ref-Guide.PDF
 */

#include "farseg.h"
#include "VGA.h"



/*******************************************************************************
 *
 *
 *	Video Graphics Adapter functions using BIOS
 *
 *
 ******************************************************************************/

void vgaFunction(int function, __dpmi_regs *regs)
{
	regs->h.ah = function;
	__dpmi_int(VGA_VIDEO_INT86, regs);
}

/*******************************************************************************
 *
 *	VGA_SET_MODE (00h)
 *
 *	Loads display adapter registers that produce a desired pixel resolution,
 *	refresh rate, color depth, and memory mapping. Many predefined mode
 *	resolutions have been built into the BIOS with an assigned mode number.
 *	Use the VGA_GET_CURRENT_VIDEO_STATE function to determine if a mode was
 *	successfully set.
 *
 ******************************************************************************/

void vgaSetMode(int mode)
{
	__dpmi_regs regs;
	regs.h.al = mode;
	vgaFunction(VGA_SET_MODE, &regs);
}

/*******************************************************************************
 *
 *	VGA_SET_CURSOR_TYPE (01h)
 *
 *	Sets the shape of the cursor by setting the starting and ending scan
 *	lines of the cursor within the 32 scan line character space. Setting the
 *	end scan line less than the start scan line will result in no cursor
 *	being displayed.
 *
 ******************************************************************************/

void vgaSetCursorType(int start, int end)
{
	__dpmi_regs regs;
	regs.h.ch = start;
	regs.h.cl = end;
	vgaFunction(VGA_SET_CURSOR_TYPE, &regs);
}

/*******************************************************************************
 *
 *	VGA_SET_CURSOR_POSITION (02h)
 *
 *	Position the text cursor at a specified location on the display screen.
 *	For modes with multiple display pages, a separate cursor, up to 8, is
 *	maintained for each display page.
 *
 ******************************************************************************/

void vgaSetCursorPosition(int page, int row, int column)
{
	__dpmi_regs regs;
	regs.h.bh = page;
	regs.h.dh = row;
	regs.h.dl = column;
	vgaFunction(VGA_SET_CURSOR_POSITION, &regs);
}

/*******************************************************************************
 *
 *	VGA_GET_CURSOR_POSITION (03h)
 *
 *	Returns the cursor position and type for a specified display page.
 *
 ******************************************************************************/

void vgaGetCursorPosition(int page, int *row, int *column, int *start, int *end)
{
	__dpmi_regs regs;
	regs.h.bh = page;
	vgaFunction(VGA_GET_CURSOR_POSITION, &regs);
	setsafe(start,  regs.h.ch);
	setsafe(end,    regs.h.cl);
	setsafe(row,    regs.h.dh);
	setsafe(column, regs.h.dl);
}

/*******************************************************************************
 *
 *	VGA_READ_LIGHT_PEN_POSITION (04h)
 *
 *	This function always returns light pen switch not activated.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	VGA_SET_ACTIVE_DISPLAY_PAGE (05h)
 *
 *	Selects the page that is displayed on the device.
 *
 ******************************************************************************/

void vgaSetActiveDisplayPage(int page)
{
	__dpmi_regs regs;
	regs.h.al = page;
	vgaFunction(VGA_SET_ACTIVE_DISPLAY_PAGE, &regs);
}

/*******************************************************************************
 *
 *	VGA_SCROLL_ACTIVE_PAGE_UP (06h)
 *
 *	Will scroll page the number or requested lines in a defined window on
 *	the active display page. Empty lines created at the bottom of the window
 *	are cleared using the given attribute. If the given number of lines to
 *	scroll is zero the entire window is cleared using the given value.
 *
 ******************************************************************************/

void vgaScrollActivePageUp(int lines, int clearing, int topRow, int topColumn, int bottomRow, int bottomColumn)
{
	__dpmi_regs regs;
	regs.h.al = lines;
	regs.h.bh = clearing;
	regs.h.ch = topRow;
	regs.h.cl = topColumn;
	regs.h.dh = bottomRow;
	regs.h.dl = bottomColumn;
	vgaFunction(VGA_SCROLL_ACTIVE_PAGE_UP, &regs);
}

/*******************************************************************************
 *
 *	VGA_SCROLL_ACTIVE_PAGE_DOWN (07h)
 *
 *	Will scroll down the number of requested lines in a defined window on
 *	the active display page. Empty lines created at the top of the window
 *	are cleared using the given attribute. If the given number of lines to
 *	scroll is zero the entire window is cleared using the given attribute.
 *
 ******************************************************************************/

void vgaScrollActivePageDown(int lines, int clearing, int topRow, int topColumn, int bottomRow, int bottomColumn)
{
	__dpmi_regs regs;
	regs.h.al = lines;
	regs.h.bh = clearing;
	regs.h.ch = topRow;
	regs.h.cl = topColumn;
	regs.h.dh = bottomRow;
	regs.h.dl = bottomColumn;
	vgaFunction(VGA_SCROLL_ACTIVE_PAGE_DOWN, &regs);
}

/*******************************************************************************
 *
 *	VGA_GET_ATTRIBUTE_AT_CURSOR (08h)
 *
 *	Returns the character ASCII code and its attribute at the current cursor
 *	position of the given display page. No attribute is returned for graphic
 *	modes.
 *
 ******************************************************************************/

void vgaGetAttributeAtCursor(int page, char *ascii, int *attribute)
{
	__dpmi_regs regs;
	regs.h.bh = page;
	vgaFunction(VGA_GET_ATTRIBUTE_AT_CURSOR, &regs);
	setsafe(ascii,     regs.h.al);
	setsafe(attribute, regs.h.ah);
}

/*******************************************************************************
 *
 *	VGA_SET_ATTRIBUTE_AT_CURSOR (09h)
 *
 *	Writes a character ASCII code and its attribute at the current cursor
 *	position. The character is written the requested number of times with
 *	some limitations. The cursor is not changed.
 *
 ******************************************************************************/

void vgaSetAttributeAtCursor(int page, char ascii, int attribute, int repeat)
{
	__dpmi_regs regs;
	regs.h.bh = page;
	regs.h.al = ascii;
	regs.h.bl = attribute;
	regs.x.cx = repeat;
	vgaFunction(VGA_SET_ATTRIBUTE_AT_CURSOR, &regs);
}

/*******************************************************************************
 *
 *	VGA_WRITE_CHAR_AT_CURSOR (0ah)
 *
 *	Write an ASCII character code to the current cursor position. The
 *	attribute is unaffected. The character is written the requested number
 *	of times with some lmitations. The cursor location is not changed. If
 *	the current mode is graphics mode, and bit d7 of register bl is 1, the
 *	character being written will be exclusive OR'ed with the previous data
 *	in display memory.
 *
 ******************************************************************************/

void vgaWriteCharAtCursor(int page, char ascii, char color, int repeat)
{
	__dpmi_regs regs;
	regs.h.al = ascii;
	regs.h.bh = page;
	regs.h.bl = color;
	regs.x.cx = repeat;
	vgaFunction(VGA_WRITE_CHAR_AT_CURSOR, &regs);
}

/*******************************************************************************
 *
 *	CGA_SET_COLOR_PALETTE (0bh)
 *
 *	Provided for CGA compatibility. Will configure the card to emulate one
 *	of the two standard CGA graphics color palettes.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	VGA_SET_PIXEL (0ch)
 *
 *	Provides a device independant method for manipulating pixels in graphics
 *	mode. Pixel value range is dependant on the color depth of the mode; for
 *	example, mode 4h is a 4 bpp mode so the range is 0-3, whereas mode dh is
 *	a 16 bpp mode so the range is 0-15
 *
 ******************************************************************************/

void vgaSetPixel(int page, int row, int column, char color)
{
	__dpmi_regs regs;
	regs.h.al = color;
	regs.h.bh = page;
	regs.x.cx = row;
	regs.x.dx = column;
	vgaFunction(VGA_SET_PIXEL, &regs);
}

/*******************************************************************************
 *
 *	VGA_GET_PIXEL (0dh)
 *
 *	Provides a device independant method for reading pixels in graphics
 *	modes.
 *
 ******************************************************************************/

char vgaGetPixel(int page, int row, int column)
{
	__dpmi_regs regs;
	regs.h.bh = page;
	regs.x.cx = row;
	regs.x.dx = column;
	vgaFunction(VGA_GET_PIXEL, &regs);
	return regs.h.al;
}

/*******************************************************************************
 *
 *	VGA_WRITE_TTY_CHAR (0eh)
 *
 *	Writes an ASCII code at the current cursor position and then
 *	automatically advances the cursor to the next character position. At the
 *	end of the line, the text automatically wraps around to the next line.
 *	At the end of the page, the screen is automatically scrolled up. The
 *	ASCII codes for BELL, BACKSPACE, CARRIAGE RETURN, and LINE FEED are all
 *	recognized and handled properly. Used by the MS-DOS driver extensively
 *	when printing characters to the display.
 *
 ******************************************************************************/

void vgaWriteTeletypeChar(int page, char ascii, char color)
{
	__dpmi_regs regs;
	regs.h.al = ascii;
	regs.h.bh = page;
	regs.h.bl = color;
	vgaFunction(VGA_WRITE_TTY_CHAR, &regs);
}

/*******************************************************************************
 *
 *	VGA_GET_CURRENT_VIDEO_STATE (0fh)
 *
 *	Returns the screen width in character columns, video display mode, and
 *	active display page.
 *
 ******************************************************************************/

void vgaGetCurrentVideoState(int *page, int *mode, int *columns)
{
	__dpmi_regs regs;
	vgaFunction(VGA_GET_CURRENT_VIDEO_STATE, &regs);
	setsafe(page,    regs.h.bh);
	setsafe(mode,    regs.h.al);
	setsafe(columns, regs.h.ah);
}

/*******************************************************************************
 *
 *	VGA_DAC (10h)
 *
 *	This function has a group of subfunctions that help in reading or
 *	writing of the video adapter palettes. The adapter palettes include the
 *	16 colors for the DAC, the border color, and the 256 RGB colors of the
 *	RAMDAC look up table. These functions allow the palette colors to be
 *	updated by index or by entire blocks.
 *
 *		(see list of VGA_DAC subfunctions)
 *
 ******************************************************************************/

void vgaDAC(int subfunction, __dpmi_regs *regs)
{
	regs->h.al = subfunction;
	vgaFunction(VGA_DAC, regs);
}

/*
 * VGA_SET_PALETTE_COLOR (00h)
 *
 *	Loads a given individual DAC color register with the given color.
 */

void vgaSetPaletteColor(int index, char color)
{
	__dpmi_regs regs;
	regs.h.bh = color;
	regs.h.bl = index;
	vgaDAC(VGA_SET_PALETTE_COLOR, &regs);
}

/*
 * VGA_SET_OVERSCAN_COLOR (01h)
 *
 *	Loads the overscan (border) color register with the given value.
 */

void vgaSetOverscanColor(char color)
{
	__dpmi_regs regs;
	regs.h.bh = color;
	vgaDAC(VGA_SET_OVERSCAN_COLOR, &regs);
}

/*
 * VGA_SET_ENTIRE_PALETTE_AND_OVERSCAN (02h)
 *
 *	Provides a fast method of programming all palette registers. A pointer
 *	to a 17 byte table must be provided; bytes 0-15 contain data for DAC
 *	palette registers 00h-0fh, and byte 16 hold the overscan (border) color.
 */

void vgaSetEntirePaletteAndOverscan(char *buffer)
{
	__dpmi_regs regs;
	/* Copy palette and overscan into transfer buffer */
	dosmemput(buffer, 17, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	vgaDAC(VGA_SET_ENTIRE_PALETTE_AND_OVERSCAN, &regs);
}

/*
 * VGA_MODE_CONTROL (03h)
 *
 *	Loads the blink/intensity field of Mode Control Register. This field
 *	defines whether bit 7 of the character attribute byte controls blinking
 *	or background intensity.
 */

void vgaModeControl(int state)
{
	__dpmi_regs regs;
	regs.h.bl = state;
	vgaDAC(VGA_MODE_CONTROL, &regs);
}

/*
 * VGA_GET_PALETTE_COLOR (07h)
 *
 *	Returns the current contents of a requested DAC palette register.
 */

char vgaGetPaletteColor(int index)
{
	__dpmi_regs regs;
	regs.h.bl = index;
	vgaDAC(VGA_GET_PALETTE_COLOR, &regs);
	return regs.h.bh;
}

/*
 * VGA_GET_OVERSCAN_COLOR (08h)
 *
 *	Returns the current contents of the overscan (border) register.
 */

char vgaGetOverscanColor(void)
{
	__dpmi_regs regs;
	vgaDAC(VGA_GET_OVERSCAN_COLOR, &regs);
	return regs.h.bh;
}

/*
 * VGA_GET_ENTIRE_PALETTE_AND_OVERSCAN (09h)
 *
 *	Read all DAC palette registers and the overscan (border) register and
 *	copy them into a given 17 byte buffer.
 */

void vgaGetEntirePaletteAndOverscan(char *buffer)
{
	__dpmi_regs regs;
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, 17, 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	vgaDAC(VGA_GET_ENTIRE_PALETTE_AND_OVERSCAN, &regs);
	dosmemget(__tb, 17, buffer);
}

/*
 * VGA_SET_PALETTE (10h)
 *
 *	Sets a requested RAMDAC look up table color to the given RGB values.
 */

void vgaSetPalette(int index, char red, char green, char blue)
{
	__dpmi_regs regs;
	regs.x.bx = index;
	regs.h.dh = red;
	regs.h.ch = green;
	regs.h.cl = blue;
	vgaDAC(VGA_SET_PALETTE, &regs);
}

/*
 * VGA_SET_PALETTE_RANGE (12h)
 *
 *	Sets a block of RAMDAC look up table color registers given a staring
 *	index, the number of colors to set, and a pointer to a table of RGB
 *	values.
 */

void vgaSetPaletteRange(int index, int count, char palette[ 256 ][ 3 ])
{
	__dpmi_regs regs;
	regs.x.bx = index;
	regs.x.cx = count * 3;
	/* Copy palette and overscan into transfer buffer */
	dosmemput(palette[ index ], count * 3, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	vgaDAC(VGA_SET_PALETTE_RANGE, &regs);
}

/*
 * VGA_SET_COLOR_PAGE (13h)
 *
 *	There are 256 color registers, but most standard vga modes do not use
 *	them all. Depending on the mode color resolution, the 256 color
 *	registers can be split up into 16 blocks with 16 color registers, or
 *	four blocks with 64 color registers. This subfunction allows an
 *	application to select how the 256 color registers are to be divided, as
 *	explained above, and then allow the application to select which of these
 *	blocks is to be made active.
 */

void vgaSetColorPage(int function, int page)
{
	__dpmi_regs regs;
	regs.h.bl = function;
	regs.h.bh = page;
	vgaDAC(VGA_SET_COLOR_PAGE, &regs);
}

/*
 * VGA_GET_PALETTE (15h)
 *
 *	Returns the contents of a requested RAMDAC look up table color register.
 */

void vgaGetPalette(int index, char *red, char *green, char *blue)
{
	__dpmi_regs regs;
	regs.x.bx = index;
	vgaDAC(VGA_GET_PALETTE, &regs);
	setsafe(red,   regs.h.dh);
	setsafe(green, regs.h.ch);
	setsafe(blue,  regs.h.cl);
}

/*
 * VGA_GET_PALETTE_RANGE (17h)
 *
 *	Returns a block of RAMDAC look up table color registers in the provided
 *	memory buffer given a starting index and the number of colors to read.
 */

void vgaGetPaletteRange(int index, int count, char palette[ 256 ][ 3 ])
{
	__dpmi_regs regs;
	regs.x.bx = index;
	regs.x.cx = count * 3;
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, count * 3, 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	vgaDAC(VGA_GET_PALETTE_RANGE, &regs);
	dosmemget(__tb, count * 3, palette[ index ]);
}

/*
 * VGA_GET_COLOR_PAGE (1ah)
 *
 *	Returns the current 256 color register paging mode and active page as
 *	set in the VGA_SET_COLOR_PAGE subfunction.
 */

int vgaGetColorPage(int function)
{
	__dpmi_regs regs;
	vgaDAC(VGA_GET_COLOR_PAGE, &regs);
	switch(function)
	{
	case VGA_PAGING_MODE:
		return regs.h.bl;
	case VGA_ACTIVE_PAGE:
		return regs.h.bh;
	}
	return -1;
}

/*
 * VGA_GRAY_SCALE_PALETTE (1bh)
 *
 *	Converts a block of RAMDAC look up table color registers from color
 *	values to monochrome gray scale values. For each register, the color
 *	data is read, a weighted sum computed, and the result written back to
 *	the register. The original color data is lost.
 */

void vgaGrayScalePalette(int index, int count)
{
	__dpmi_regs regs;
	regs.x.bx = index;
	regs.x.cx = count;
	vgaDAC(VGA_GRAY_SCALE_PALETTE, &regs);
}

/*******************************************************************************
 *
 *	VGA_CHAR_GENERATOR (11h)
 *
 *	This function has a group of subfunctions that help deal with internal
 *	video BIOS and user font sets. Two sets of calls are provided for use
 *	with text modes. The first simply loads the requested font in plane 2 of
 *	the frame buffer memory. The second loads the font in plane 2 and then
 *	adjusts certain display parameters.
 *
 *	For text modes, individual characters of a font can be up to 32 bytes in
 *	length. The font data reserved in display memory can hold up to 8 256
 *	character set fonts. The font haracters of any length are loaded on 32
 *	byte boundaries for easy access. Therefore, a 256 character font will
 *	take up to 8kb (8192 bytes). The active character set in the font area
 *	is selected through vga sequencer register SR03.
 *
 *		(see list of VGA_CHAR_GENERATOR subfunctions)
 *
 ******************************************************************************/

void vgaCharGenerator(int subfunction, __dpmi_regs *regs)
{
	regs->h.al = subfunction;
	vgaFunction(VGA_CHAR_GENERATOR, regs);
}

/*
 * VGA_LOAD_USER_FONT (00h)
 *
 *	Loads a user defined font or a portion of it into one of eight font
 *	areas reserved in text mode memory. This function does not recaculate
 *	the CRT controller registers
 */

void vgaLoadUserFont(int buffer, int numChar, int offset, int bytesPerChar, void *table)
{
	__dpmi_regs regs;
	regs.h.bh = bytesPerChar;
	regs.h.bl = buffer;
	regs.x.cx = numChar;
	regs.x.dx = offset;
	/* Write string to the transfer buffer */
	dosmemput(table, numChar * bytesPerChar, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.bp = far2off(__tb);
	vgaCharGenerator(VGA_LOAD_USER_FONT, &regs);
}

/*
 * VGA_8x14_FONT (01h)
 *
 *	Will load the video BIOS 8x14 font into the specified font area in
 *	display memory. This font is mainly a monochrome EGA font. To save space
 *	on a video BIOS this font is sometimes removed and replaced with a
 *	conversion from the 8x16 font. This function does not recalculate the
 *	CRT controller registers.
 *
 * VGA_8x8_FONT (02h)
 *
 *	Will load the video BIOS 8x8 font into the specified font area in
 *	display memory.
 *
 * VGA_8x16_FONT (04h)
 *
 *	Will load the video BIOS 8x16 font into the specified font area in
 *	display memory.
 */

void vgaLoadROMFont(int buffer, int font)
{
	__dpmi_regs regs;
	regs.h.bl = buffer;
	vgaCharGenerator(font, &regs);
}

/*
 * VGA_SET_FONT_BLOCK_SPECIFIER (03h)
 *
 *	Allows the application to select which of the 8 internal character
 *	generator tables (font buffers) are active. Before a table can be made
 *	active it must be loaded with character data.
 */

/*
 * VGA_GET_FONT_INFO (30h)
 *
 *	Returns a pointer to the character definition table for a font. It also
 *	returns the bytes per character and character rows for that font.
 */

int vgaGetFontInfo(int font, void *buffer)
{
	__dpmi_regs regs;
	regs.h.bh = font;
	vgaCharGenerator(VGA_GET_FONT_INFO, &regs);
	dosmemget(near3far(regs.x.es, regs.x.bp), 128 * regs.x.cx, buffer);
	return regs.x.cx;
}

/*******************************************************************************
 *
 *	VGA_ALTERNATE_SELECT (12h)
 *
 *	Performs a variety of seemingly unrelated tasks. These tasks range from
 *	returning video information to selecting the number of scan lines in a
 *	mode resolution.
 *
 *		(see list of VGA_ALTERNATE_SELECT subfunctions)
 *
 ******************************************************************************/

void vgaAlternateSelect(int subfunction, __dpmi_regs *regs)
{
	regs->h.bl = subfunction;
	vgaFunction(VGA_ALTERNATE_SELECT, regs);
}

/*
 * VGA_GET_INFO (10h)
 *
 *	Returns information on the current vga configuration. Although most of
 *	this information is no longer applicable, it is still returned for
 *	backward compatibility.
 */

int vgaGetInfo(VGAreturns result)
{
	__dpmi_regs regs;
	vgaAlternateSelect(VGA_GET_INFO, &regs);
	switch(result)
	{
	case VGA_COLOR_MODE      :
		return regs.h.bh == 0x00;
	case VGA_MONOCHROME_MODE :
		return regs.h.bh == 0x01;
	case VGA_FEATURE_BITS    :
		return regs.h.ch;
	case VGA_SWITCH_SETTING  :
		return regs.h.cl;
	}
	return -1;
}

/*
 * VGA_SET_PRINT_SCREEN_ROUTINE (20h)
 *
 *	Selects an alternate print screen routine that works properly if the
 *	number of character lines is not the normal 25. The ROM BIOS default
 *	print screen routine always prints 25 lines.
 */

/*
 * VGA_SET_TEXT_MODE_SCAN_LINES (30h)
 *
 *	Selects the number of scan lines for text modes. The selected value
 *	takes effect the next time VGA_SET_MODE is called.
 */

boolean vgaSetTextModeScanLines(int scanlines)
{
	__dpmi_regs regs;
	regs.h.al = scanlines;
	vgaAlternateSelect(VGA_SET_TEXT_MODE_SCAN_LINES, &regs);
	return regs.h.ah == VGA_ALTERNATE_SELECT;
}

/*
 * VGA_DEFAULT_PALETTE_LOADING (31h)
 *
 *	Enables or disables loading of a default palette when VGA_SET_MODE is
 *	called.
 *
 * VGA_VIDEO_ADAPTER (32h)
 *
 *	Enables or disables the video adapter from responding to any I/O or
 *	memory reads or writes. The display is not affected.
 *
 * VGA_GRAY_SCALE_SUMMING (33h)
 *
 *	Enables or disables the gray scale summing feature. If enabled, gray
 *	scale summing will be performed to all 256 RAMDAC colors whenever the
 *	color registers are loaded through video BIOS functions.
 *
 * VGA_CURSOR_EMULATION (34h)
 *
 *	Addresses the cursor compatibility problem when CGA software that sets
 *	cursor shape is used with EGA or vga text modes. Because of the larger
 *	character cell, the cursor will appear in the wrong part of the cell
 *	unless CGA cursor emulation is enabled to translate the cursor
 *	parameters to different values.
 *
 * VGA_VIDEO_SCREEN (35h)
 *
 *	Turns on or off the video refresh on the display devices.
 */

boolean vgaStateOption(int option, int state)
{
	__dpmi_regs regs;
	regs.h.al = state;
	vgaAlternateSelect(option, &regs);
	return regs.h.ah == VGA_ALTERNATE_SELECT;
}

/*
 * VGA_SWITCH_ACTIVE_DISPLAY (35h)
 *
 *	Allows selection between 1 of 2 video adapters in the system when memory
 *	usage or port addresses conflict between the two adapters.
 */

boolean vgaSwitchActiveDisplay(int function, void *buffer)
{
	__dpmi_regs regs;
	regs.h.al = function;
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, 128, 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	vgaAlternateSelect(VGA_SWITCH_ACTIVE_DISPLAY, &regs);
	return regs.h.ah == VGA_ALTERNATE_SELECT;
}

/*******************************************************************************
 *
 *	VGA_WRITE_STRING (13h)
 *
 *	Allows an application to pass an entire text string to the BIOS for
 *	display given a pointer to the string. The string may or may not include
 *	embedded attribute data. ASCII characters for BELL, BACKSPACE, CARRIAGE
 *	RETURNS, and LINEFEED are all recognized and handled accordingly.
 *
 ******************************************************************************/

void vgaWriteString(int page, int flags, char color, int row, int column, const char *string, int length)
{
	__dpmi_regs regs;
	regs.h.al = flags;
	regs.h.bh = page;
	regs.h.bl = color;
	regs.x.cx = length;
	regs.h.dh = row;
	regs.h.dl = column;
	/* Write string to the transfer buffer */
	dosmemput(string, length, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.bp = far2off(__tb);
	vgaFunction(VGA_WRITE_STRING, &regs);
}

/*******************************************************************************
 *
 *	VGA_DISPLAY_COMBINATION_CODE (1ah)
 *
 *	This function has separate read and write subfunctions that deal with
 *	information on the current installed display adapters.
 *
 ******************************************************************************/

int vgaGetDCC(vgaReturns status)
{
	__dpmi_regs regs;
	regs.h.al = VGA_GET;
	vgaFunction(VGA_DISPLAY_COMBINATION_CODE, &regs);
	if(regs.h.al == VGA_DISPLAY_COMBINATION_CODE)
	{
		switch(status)
		{
		case VGA_INACTIVE :
			return regs.h.bh;
		case VGA_ACTIVE   :
			return regs.h.bl;
		}
	}
	return -1;
}

boolean vgaSetDCC(int active, int inactive)
{
	__dpmi_regs regs;
	regs.h.al = VGA_SET;
	regs.h.bh = inactive;
	regs.h.bl = active;
	vgaFunction(VGA_DISPLAY_COMBINATION_CODE, &regs);
	return regs.h.al == VGA_DISPLAY_COMBINATION_CODE;
}

/*******************************************************************************
 *
 *	VGA_GET_STATE_INFO (1bh)
 *
 *	Returns functionality and state information in a provided memory buffer
 *	for the current adapter and mode. See the structures 'VGAstateInfo' and
 *	'VGAstatic' for details on the data returned.
 *
 ******************************************************************************/

boolean vgaGetStateInfo(VGAstateInfo *state)
{
	__dpmi_regs regs;
	regs.x.bx = 0x00; /* Implementation type? */
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, sizeof(VGAstateInfo), 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.di = far2off(__tb);
	vgaFunction(VGA_GET_STATE_INFO, &regs);
	if(regs.h.ah == VGA_GET_STATE_INFO)
	{
		/* Copy data from the transfer buffer into VGAstateInfo */
		dosmemget(__tb, sizeof(VGAstateInfo), state);
		return TRUE;
	}
	return FALSE;
}

/*******************************************************************************
 *
 *	VGA_VIDEO_STATE (1ch)
 *
 *	This function and it's set of subfunctions save and restore video
 *	environment parameters (BIOS data area, color palette, and video adapter
 *	registers). The video BIOS has extended this function to include the
 *	extended state.
 *
 *		(see list of VGA_VIDEO_STATE subfunctions)
 *
 ******************************************************************************/

boolean vgaVideoState(int subfunction, int flags, __dpmi_regs *regs)
{
	regs->h.al = subfunction;
	regs->x.cx = flags;
	vgaFunction(VGA_VIDEO_STATE, regs);
	return regs->h.al == VGA_VIDEO_STATE;
}

/*
 * VGA_QUERY (00h)
 *
 *	Returns the minimum buffer size (in 64 byte blocks) required to hold the
 *	data loaded by the save and restore functions. The size of this buffer
 *	is updated according to the save/restore options selected in cx.
 */

int vgaQueryStateBufferSize(int flags)
{
	__dpmi_regs regs;
	if(vgaVideoState(VGA_QUERY, flags, &regs))
		/* Size is given in 64 byte blocks, but we want raw bytes */
		return regs.x.bx * 64;
	else
		return -1;
}

/*
 * VGA_SAVE (01h)
 *
 *	Loads the given buffer with the requested state data.
 */

boolean vgaSaveState(int flags, void *buffer, int length)
{
	__dpmi_regs regs;
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, length, 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.bx = far2off(__tb);
	if(vgaVideoState(VGA_SAVE, flags, &regs))
	{
		/* Copy data from the transfer buffer into allocated memory */
		dosmemget(__tb, length, buffer);
		return TRUE;
	}
	return FALSE;
}

/*
 * VGA_RESTORE (02h)
 *
 *	Restores a previously saved state from the given buffer.
 */

boolean vgaRestoreState(int flags, void *buffer, int length)
{
	__dpmi_regs regs;
	/* Copy data from allocated memory into transfer buffer */
	dosmemput(buffer, length, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.bx = far2off(__tb);
	return vgaVideoState(VGA_RESTORE, flags, &regs);
}



