/*******************************************************************************
 *
 *	VGA Input/Output Registers
 *
 *	Portions of code from David Brackeen
 *	http://www.brackeen.com/home/vga/
 *	Portions of code from Brennan (?)
 *	http://www.delorie.com/djgpp/doc/brennan/brennan_access_vga.html
 *	Additional information provided by Xavier Leclercq
 *	http://www.xaff.org/VGA/vga.html
 *	Additional information provided by FreeVGA & J.D.Neal
 *	http://www.osdever.net/FreeVGA/home.htm
 */

#ifndef VGAio_h
#define VGAio_h

#include "VGA.h"

/*******************************************************************************
 *
 *	Video Graphics Adapter I/O ports
 *
 ******************************************************************************/

#define VGA_MONO_CRTC_ADDRESS				0x3B4
#define VGA_MONO_CRTC_DATA				0x3B5
#define VGA_MONO_INPUT_STATUS_1				0x3Ba
#define VGA_MONO_FEATURE_CONTROL_WRITE			0x3Ba
#define VGA_ATTRIBUTE_ADDRESS				0x3C0
#define VGA_ATTRIBUTE_DATA_WRITE			0x3C0
#define VGA_ATTRIBUTE_DATA_READ				0x3C1
#define VGA_INPUT_STATUS_0				0x3C2
#define VGA_MISC_OUTPUT_WRITE				0x3C2
#define VGA_SEQUENCER_ADDRESS				0x3C4
#define VGA_SEQUENCER_DATA				0x3C5
#define VGA_DAC_STATE					0x3C7
#define VGA_DAC_ADDRESS_READ_MODE			0x3C7
#define VGA_DAC_ADDRESS_WRITE_MODE			0x3C8
#define VGA_DAC_DATA					0x3C9
#define VGA_FEATURE_CONTROL_READ			0x3CA
#define VGA_MISC_OUTPUT_READ				0x3CC
#define VGA_GRAPHICS_ADDRESS				0x3CE
#define VGA_GRAPHICS_DATA				0x3CF
#define VGA_COLOR_CRTC_ADDRESS				0x3D4
#define VGA_COLOR_CRTC_DATA				0x3D5
#define VGA_COLOR_INPUT_STATUS_1			0x3DA
#define VGA_COLOR_FEATURE_CONTROL_WRITE 		0x3DA

/* Determined at runtime (Monochrome/Color) */

static int VGA_CRTC_ADDRESS				= -1;
static int VGA_CRTC_DATA				= -1;
static int VGA_INPUT_STATUS_1				= -1;
static int VGA_FEATURE_CONTROL_WRITE			= -1;

/*******************************************************************************
 *
 *	Video Graphics Adapter I/O registers
 *
 ******************************************************************************/

/* VGA_ATTRIBUTE_ADDRESS_x */

#define VGA_ATTRIBUTE_MODE_CONTROL			0x10
#define VGA_OVERSCAN_COLOR				0x11
#define VGA_COLOR_PLANE					0x12
#define VGA_HORIZONTAL_PIXEL_PANNING			0x13
#define VGA_COLOR_SELECT				0x14

/* VGA_CRTC_x */

#define VGA_HORIZONTAL_TOTAL				0x00
#define VGA_END_HORIZONTAL_DISPLAY			0x01
#define VGA_START_HORIZONTAL_BLANKING			0x02
#define VGA_END_HORIZONTAL_BLANKING			0x03
#define VGA_START_HORIZONTAL_RETRACE			0x04
#define VGA_END_HORIZONTAL_RETRACE			0x05
#define VGA_VERTICAL_TOTAL				0x06
#define VGA_OVERFLOW					0x07
#define VGA_PRESET_ROW_SCAN				0x08
#define VGA_MAXIMUM_SCAN_LINE				0x09
#define VGA_CURSOR_START				0x0A
#define VGA_CURSOR_END					0x0B
#define VGA_START_ADDRESS_HIGH				0x0C
#define VGA_START_ADDRESS_LOW				0x0D
#define VGA_CURSOR_LOCATION_HIGH			0x0E
#define VGA_CURSOR_LOCATION_LOW				0x0F
#define VGA_VERTICAL_RETRACE_START			0x10
#define VGA_VERTICAL_RETRACE_END			0x11
#define VGA_VERTICAL_DISPLAY_END			0x12
#define VGA_OFFSET					0x13
#define VGA_UNDERLINE_LOCATION				0x14
#define VGA_START_VERTICAL_BLANKING			0x15
#define VGA_END_VERTICAL_BLANKING			0x16
#define VGA_CRTC_MODE_CONTROL				0x17
#define VGA_LINE_COMPARE				0x18

/* VGA_SEQUENCER_x */

#define VGA_RESET					0x00
#define VGA_CLOCKING_MODE				0x01
#define VGA_MAP_MASK					0x02
#define VGA_CHARACTER_MAP_SELECT			0x03
#define VGA_SEQUENCER_MEMORY_MODE			0x04

/* VGA_GRAPHICS_x */

/* #define VGA_RESET					0x00 */
#define VGA_ENABLE_RESET				0x01
#define VGA_COLOR_COMPARE				0x02
#define VGA_DATA_ROTATE					0x03
#define VGA_READ_MAP_SELECT				0x04
#define VGA_GRAPHICS_MODE				0x05
#define VGA_MISC_GRAPHICS				0x06
#define VGA_COLOR_DONT_CARE				0x07
#define VGA_BIT_MASK					0x08

/*******************************************************************************
 *
 *	Video Graphics Adapter I/O bits & masks
 *
 ******************************************************************************/

/****** VGA_ATTRIBUTE_ADDRESS_x ***********************************************/

#define VGA_ATTRIBUTE_ADDRESS_BIT			0x1F	/* 00011111 */
#define VGA_PALETTE_ADDRESS_SOURCE_BIT			0x20	/* 00100000 */

/* VGA_ATTRIBUTE_MODE_CONTROL */

#define VGA_ATTRIBUTE_CONTROLLER_GRAPHICS_ENABLE_BIT	0x01	/* 00000001 */
#define VGA_MONOCHROME_EMULATION_BIT			0x02	/* 00000010 */
#define VGA_LINE_GRAPHICS_ENABLE_BIT			0x04	/* 00000100 */
#define VGA_BLINK_ENABLE_BIT				0x08	/* 00001000 */
#define VGA_PIXEL_PANNING_MODE_BIT			0x20	/* 00100000 */
#define VGA_8_BIT_COLOR_ENABLE_BIT			0x40	/* 01000000 */
#define VGA_PALETTE_BITS_5_4_SELECT_BIT			0x80	/* 10000000 */

/* VGA_OVERSCAN_COLOR */

#define VGA_OVERSCAN_PALETTE_INDEX_BIT			0xFF	/* 11111111 */

/* VGA_COLOR_PLANE_ENABLE */

#define VGA_COLOR_PLANE_BIT				0x0F	/* 00001111 */

/* VGA_HORIZONTAL_PIXEL_PANNING */

#define VGA_PIXEL_SHIFT_COUNT_BIT			0x0F	/* 00001111 */

/* VGA_COLOR_SELECT */

#define VGA_COLOR_SELECT_5_4_BIT			0x03	/* 00000011 */
#define VGA_COLOR_SELECT_7_6_BIT			0x0C	/* 00001100 */

/****** VGA_CRTC_x ************************************************************/

/* VGA_HORIZONTAL_TOTAL */

#define VGA_HORIZONTAL_TOTAL_BIT			0xFF	/* 11111111 */

/* VGA_END_HORIZONTAL_DISPLAY */

#define VGA_END_HORIZONTAL_DISPLAY_BIT			0xFF	/* 11111111 */

/* VGA_START_HORIZONTAL_BLANKING */

#define VGA_START_HORIZONTAL_BLANKING_BIT		0xFF	/* 11111111 */

/* VGA_END_HORIZONTAL_BLANKING */

#define VGA_END_HORIZONTAL_BLANKING_BIT			0x1F	/* 00011111 */
#define VGA_DISPLAY_SKEW_ENABLE_BIT			0x60	/* 01100000 */
#define VGA_VERTICAL_RETRACE_ACCESS_ENABLE_BIT		0x80	/* 10000000 */

/* VGA_START_HORIZONTAL_RETRACE */

#define VGA_START_HORIZONTAL_RETRACE_BIT		0xFF	/* 11111111 */

/* VGA_END_HORIZONTAL_RETRACE */

#define VGA_END_HORIZONTAL_RETRACE_BIT			0x1F	/* 00011111 */
#define VGA_HORIZONTAL_RETRACE_SKEW_BIT			0x60	/* 01100000 */
#define VGA_END_HORIZONTAL_BLANKING_BIT_5		0x80	/* 10000000 */

/* VGA_VERTICAL_TOTAL */

#define VGA_VERTICAL_TOTAL_BIT				0xFF	/* 11111111 */

/* VGA_OVERFLOW */

#define VGA_VERTICAL_TOTAL_BIT_8			0x01	/* 00000001 */
#define VGA_VERTICAL_DISPLAY_END_BIT_8			0x02	/* 00000010 */
#define VGA_VERTICAL_RETRACE_START_BIT_8		0x04	/* 00000100 */
#define VGA_START_VERTICAL_BLANKING_BIT_8		0x08	/* 00001000 */
#define VGA_LINE_COMPARE_BIT_8				0x10	/* 00010000 */
#define VGA_VERTICAL_TOTAL_BIT_9			0x20	/* 00100000 */
#define VGA_VERTICAL_DISPLAY_END_BIT_9			0x40	/* 01000000 */
#define VGA_VERTICAL_RETRACE_START_BIT_9		0x80	/* 10000000 */

/* VGA_PRESET_ROW_SCAN */

#define VGA_PRESET_ROW_SCAN_BIT				0x1F	/* 00011111 */
#define VGA_BYTE_PANNING_BIT				0x60	/* 01100000 */

/* VGA_MAXIMUM_SCAN_LINE */

#define VGA_MAXIMUM_SCAN_LINE_BIT			0x60	/* 00011111 */
#define VGA_START_VERTICAL_BLANKING_BIT_9		0x20	/* 00100000 */
#define VGA_LINE_COMPARE_BIT_9				0x40	/* 01000000 */
#define VGA_SCAN_DOUBLING_BIT				0x80	/* 10000000 */

/* VGA_CURSOR_START */

#define VGA_CURSOR_SCAN_LINE_START_BIT			0x1F	/* 00011111 */
#define VGA_CURSOR_DISABLE_BIT				0x20	/* 00100000 */

/* VGA_CURSOR_END */

#define VGA_CURSOR_SCAN_LINE_END_BIT			0x1F	/* 00011111 */
#define VGA_CURSOR_SKEW_BIT				0x60	/* 01100000 */

/* VGA_START_ADDRESS_HIGH */

#define VGA_START_ADDRESS_HIGH_BIT			0xFF	/* 11111111 */

/* VGA_START_ADDRESS_LOW */

#define VGA_START_ADDRESS_LOW_BIT			0xFF	/* 11111111 */

/* VGA_CURSOR_LOCATION_HIGH */

#define VGA_CURSOR_LOCATION_HIGH_BIT			0xFF	/* 11111111 */

/* VGA_CURSOR_LOCATION_LOW */

#define VGA_CURSOR_LOCATION_LOW_BIT			0xFF	/* 11111111 */

/* VGA_CURSOR_LOCATION_LOW */

#define VGA_CURSOR_LOCATION_LOW_BIT			0xFF	/* 11111111 */

/* VGA_VERTICAL_RETRACE_START */

#define VGA_VERTICAL_RETRACE_START_BIT			0xFF	/* 11111111 */

/* VGA_VERTICAL_RETRACE_END */

#define VGA_VERTICAL_RETRACE_END_BIT			0x0F	/* 00001111 */
#define VGA_MEMORY_REFRESH_BANDWIDTH_BIT		0x40	/* 01000000 */
#define VGA_CRTC_REGISTERS_PROTECT_ENABLE_BIT		0x80	/* 10000000 */

/* VGA_VERTICAL_DISPLAY_END */

#define VGA_VERTICAL_DISPLAY_END_BIT			0xFF	/* 11111111 */

/* VGA_OFFSET */

#define VGA_OFFSET_BIT					0xFF	/* 11111111 */

/* VGA_UNDERLINE_LOCATION */

#define VGA_UNDERLINE_LOCATION_BIT			0x1F	/* 00011111 */
#define VGA_DIVIDE_MEMORY_ADDRESS_CLOCK_4_BIT		0x20	/* 00100000 */
#define VGA_DOUBLE_WORD_ADDRESSING_BIT			0x40	/* 01000000 */

/* VGA_START_VERTICAL_BLANKING */

#define VGA_START_VERTICAL_BLANKING_BIT			0xFF	/* 11111111 */

/* VGA_END_VERTICAL_BLANKING */

#define VGA_END_VERTICAL_BLANKING_BIT			0xEF	/* 01111111 */

/* VGA_CRTC_MODE_CONTROL */

#define VGA_MAP_DISPLAY_ADDRESS_13_BIT			0x01	/* 00000001 */
#define VGA_MAP_DISPLAY_ADDRESS_14_BIT			0x02	/* 00000010 */
#define VGA_DIVIDE_SCAN_LINE_CLOCK_2_BIT		0x04	/* 00000100 */
#define VGA_DIVIDE_MEMORY_ADDRESS_CLOCK_2_BIT		0x08	/* 00001000 */
#define VGA_ADDRESS_WRAP_SELECT_BIT			0x20	/* 00100000 */
#define VGA_WORD_BYTE_MODE_SELECT_BIT			0x40	/* 01000000 */
#define VGA_SYNC_ENABLE_BIT				0x80	/* 10000000 */

/* VGA_LINE_COMPARE */

#define VGA_LINE_COMPARE_BIT				0xFF	/* 11111111 */

/****** VGA_MISC_OUTPUT_x *****************************************************/

#define VGA_IO_ADDRESS_SELECT_BIT			0x01	/* 00000001 */
#define VGA_RAM_ENABLE_BIT				0x02	/* 00000010 */
#define VGA_CLOCK_SELECT_BIT				0x0c	/* 00001100 */
#define VGA_25_MHZ_CLOCK_BIT				0x00	/* ----00-- */
#define VGA_28_MHZ_CLOCK_BIT				0x04	/* ----01-- */
#define VGA_PAGE_SELECT_BIT				0x20	/* 00100000 */
#define VGA_HORIZONTAL_SYNC_POLARITY_BIT		0x40	/* 01000000 */
#define VGA_VERTICAL_SYNC_POLARITY_BIT			0x80	/* 10000000 */

/****** VGA_FEATURE_CONTROL_x *************************************************/

#define VGA_FEATURE_CONTROL_2_BIT_1			0x01	/* 00000001 */
#define VGA_FEATURE_CONTROL_1_BIT_0			0X01	/* 00000010 */

/****** VGA_INPUT_STATUS_1 ****************************************************/

#define VGA_DISPLAY_DISABLED_BIT			0x01	/* 00000001 */
#define VGA_VERTICAL_RETRACE_BIT			0x08	/* 00001000 */

/****** VGA_ATTRIBUTE_ADDRESS *************************************************/

#define VGA_ATTRIBUTE_ADDRESS_BIT			0x1F	/* 00011111 */
#define VGA_PALETTE_ADDRESS_SOURCE_BIT			0x20	/* 00100000 */

/****** VGA_INPUT_STATUS_0 ****************************************************/

#define VGA_SWITCH_SENSE_BIT				0x10	/* 00010000 */

/****** VGA_SEQUENCER_x	*******************************************************/

/* VGA_RESET */

#define VGA_ASYNCHRONOUS_BIT				0x01	/* 00000001 */
#define VGA_SYNCHRONOUS_BIT				0x02	/* 00000010 */

/* VGA_CLOCKING_MODE */

#define VGA_9_8_DOT_MODE_BIT				0x01	/* 00000001 */
#define VGA_9_DOTS_PER_CHAR_BIT				0x00	/* -------0 */
#define VGA_8_DOTS_PER_CHAR_BIT				0x01	/* -------1 */
#define VGA_SHIFT_LOAD_RATE_BIT				0x04	/* 00000100 */
#define VGA_DOT_CLOCK_RATE_BIT				0x08	/* 00001000 */
#define VGA_SHIFT_FOUR_ENABLE_BIT			0x10	/* 00010000 */
#define VGA_SCREEN_DISABLE_BIT				0x20	/* 00100000 */

/* VGA_MAP_MASK */

#define VGA_MAP_MASK_BIT				0x1F	/* 00001111 */
#define VGA_PLANE_0_BIT					0x01	/* ----0001 */
#define VGA_PLANE_1_BIT					0x02	/* ----0010 */
#define VGA_PLANE_2_BIT					0x04	/* ----0100 */
#define VGA_PLANE_4_BIT					0x08	/* ----1000 */

/* VGA_CHAR_MAP_SELECT */

#define VGA_CHAR_SET_B_SELECT_BIT			0x03	/* 00000011 */
#define VGA_CHAR_SET_A_SELECT_BIT			0x0C	/* 00001100 */
#define VGA_CHAR_SET_B_SELECT_BIT_2			0x10	/* 00010000 */
#define VGA_CHAR_SET_A_SELECT_BIT_2			0x20	/* 00100000 */

/* VGA_SEQUENCER_MEMORY_MODE */

#define VGA_EXTENDED_MEMORY_BIT				0x02	/* 00000010 */
#define VGA_HOST_MEMORY_WRITE_ADDRESSING_DISABLE_BIT	0x04	/* 00000100 */
#define VGA_CHAIN_4_ENABLE_BIT				0x08	/* 00001000 */

/****** VGA_DAC_DATA **********************************************************/

#define VGA_DAC_DATA_BIT				0x3F	/* 00111111 */

/****** VGA_DAC_STATE *********************************************************/

#define VGA_DAC_STATE_BIT				0x03	/* 00000011 */
#define VGA_DAC_ACCEPTING_READ_BIT			0x00	/* ------00 */
#define VGA_DAC_ACCEPTING_WRITE_BIT			0x03	/* ------11 */

/****** VGA_GRAPHICS_x ********************************************************/

/* VGA_RESET */
/* VGA_RESET_ENABLE */
/* VGA_COLOR_COMPARE */

/* #define VGA_PLANE_0_BIT				0x01 */	/* 00000001 */
/* #define VGA_PLANE_1_BIT				0x02 */	/* 00000010 */
/* #define VGA_PLANE_2_BIT				0x04 */	/* 00000100 */
/* #define VGA_PLANE_4_BIT				0x08 */	/* 00001000 */

/* VGA_DATA_ROTATE */

#define VGA_ROTATE_COUNT_BIT				0x07	/* 00000011 */
#define VGA_LOGIC_OP_BIT				0x0C	/* 00001100 */
#define VGA_LOGIC_OP_UNMODIFIED_BIT			0x00	/* ----00-- */
#define VGA_LOGIC_OP_AND_BIT				0x08	/* ----01-- */
#define VGA_LOGIC_OP_OR_BIT				0x10	/* ----10-- */
#define VGA_LOGIC_OP_XOR_BIT				0x18	/* ----11-- */

/* VGA_READ_MAP_SELECT */

#define VGA_READ_MAP_SELECT_BIT				0x03	/* 00000011 */

/* VGA_GRAPHICS_MODE */

#define VGA_WRITE_MODE_BIT				0x03	/* 00000011 */
#define VGA_READ_MODE_BIT				0x08	/* 00001000 */
#define VGA_HOST_MEMORY_READ_ADDRESSING_BIT		0x10	/* 00010000 */
#define VGA_SHIFT_REGISTER_INTERLEAVE_MODE_BIT		0x20	/* 00100000 */
#define VGA_256_COLOR_SHIFT_MODE_BIT			0x40	/* 01000000 */

/* VGA_MISC_GRAPHICS */

#define VGA_ALPHANUMERIC_MODE_DISABLE_BIT		0x01	/* 00000001 */
#define VGA_CHAIN_ENABLE_BIT				0x02	/* 00000010 */
#define VGA_ODD_CHAIN_BIT				0x02	/* ------1- */
#define VGA_EVEN_CHAIN_BIT				0x00	/* ------0- */
#define VGA_MEMORY_MAP_SELECT_BIT			0x0c	/* 00001100 */
#define VGA_A0000_BFFFF_128K_BIT			0x00	/* ----00-- */
#define VGA_A0000_AFFFF_64K_BIT				0x10	/* ----01-- */
#define VGA_B0000_B7FFF_32K_BIT				0x20	/* ----10-- */
#define VGA_B8000_BFFFF_32K_BIT				0x40	/* ----11-- */

/* VGA_COLOR_DONT_CARE */

#define VGA_COLOR_DONT_CARE_BIT				0x0F	/* 00001111 */

/* VGA_BIT_MASK */

#define VGA_BIT_MASK_BIT				0xFF	/* 11111111 */

/*******************************************************************************
 *
 *	Video Graphics Adapter I/O functions
 *
 ******************************************************************************/

/* Color/Monochrome Registers */

BOOL vgaIsColorMode();

void vgaResolveCRTCAddresses();

/* Synchronous Vertical Retrace */

void vgaOnSync();

/* Display Attribute Controller */

void vgaWritePalette(int index, char red, char green, char blue);

void vgaWritePaletteRange(char palette[256][3], int index, int count);

void vgaWriteEntirePalette(char palette[256][3]);

void vgaReadPalette(int index, char *red, char *green, char *blue);

void vgaReadPaletteRange(char palette[256][3], int index, int count);

void vgaReadEntirePalette(char palette[256][3]);

/* Query */

int vgaQuery(int port, int index, int data);

int vgaQueryCRTC(int index);

int vgaQueryAttribute(int index);

int vgaQuerySequencer(int index);

int vgaQueryGraphics(int index);

/* Query Graphics */

int vgaQueryMiscGraphics(int mask);



#endif /* VGAio_h */
