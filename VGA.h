/******************************************************************************
 *
 *	VGA library
 *
 *	Portions of code from David Brackeen
 *	http://www.brackeen.com/home/vga/
 *	Portions of code from Brennan Underwood
 *	http://www.delorie.com/djgpp/doc/brennan/brennan_access_vga.html
 *	Additional information provided by Roger Morgan
 *	http://www.htl-steyr.ac.at/~morg/pcinfo/hardware/interrupts/inte1at0.htm
 *	Additional information provided by Xavier Leclercq
 *	http://www.xaff.org/VGA/vga.html
 *	Additional information provided by FreeVGA & J.D.Neal
 *	http://www.osdever.net/FreeVGA/home.htm
 *	Additional information provided by Intel
 *	http://www.msc-ge.com/download/pc-system/ipc-produkte/pisa/VGA%20BIOS%20CT%2069030%20Ref-Guide.PDF
 */

#ifndef VGA_h
#define VGA_h

/*
 * Tells compiler to allocate structure with data fields packed to bit or byte
 * alignment. This is needed since the VGA BIOS which will be queried for this
 * info will return it with this type of alignment. The code generated  by this
 * compiler won't have any control over the origin of these structures.
 */

#pragma pack(1)

/******************************************************************************
 *
 *	Video Graphics Adapter BIOS functions
 *
 *****************************************************************************/

#define VGA_VIDEO_INT86				0x10	// Video BIOS interrupt number

#define VGA_NO_CLEAR				0x80	// Don't clear video memory 
#define VGA_XOR_CHAR				0x80	// Exclusive-or a character to its display 

#define VGA_SET_MODE				0x00	// Load display adapter registers 
#define VGA_SET_CURSOR_TYPE			0x01	// Set cursor shape 
#define VGA_SET_CURSOR_POSITION			0x02	// Set text cursor position 
#define VGA_GET_CURSOR_POSITION			0x03	// Get text cursor position 
#define VGA_GET_LIGHT_PEN_POSITION		0x04	// Always returns light pen switch not activated 
#define VGA_SET_ACTIVE_DISPLAY_PAGE		0x05	// Select which page is displayed on device 
#define VGA_SCROLL_ACTIVE_PAGE_UP		0x06	// Scrolls up requested number of lines 
#define VGA_SCROLL_ACTIVE_PAGE_DOWN		0x07	// Scrolls down requested number of lines 
#define VGA_GET_ATTRIBUTE_AT_CURSOR		0x08	// Read an attribute at cursor 
#define VGA_SET_ATTRIBUTE_AT_CURSOR		0x09	// Write an attribute at cursor 
#define VGA_WRITE_CHAR_AT_CURSOR		0x0a	// Write a character at cursor 
#define CGA_SET_COLOR_PALETTE			0x0b	// Emulate 1 of 2 CGA palettes 
#define VGA_SET_PIXEL				0x0c	// Write pixel 
#define VGA_GET_PIXEL				0x0d	// Read pixel 
#define VGA_WRITE_TTY_CHAR			0x0e	// Writes a character, advances text cursor 
#define VGA_GET_CURRENT_VIDEO_STATE		0x0f	// Read screen width, mode, and page 
#define VGA_DAC					0x10	// Device Attribute Controller 
#define VGA_CHAR_GENERATOR			0x11	// Font 
#define VGA_ALTERNATE_SELECT			0x12	// Extended registers 
#define VGA_WRITE_STRING			0x13	// Draw string of characters on display 
#define VGA_LOAD_LCD_CHARACTER_FONT		0x14	// ? 
#define VGA_GET_PHYSICAL_DISPLAY_PARAMETERS	0x15	// ? 
#define VGA_DISPLAY_COMBINATION_CODE		0x1a	// Display combination code 
#define VGA_GET_STATE_INFO			0x1b	// Return functionality/state information 
#define VGA_VIDEO_STATE				0x1c	// Video state information 

/******************************************************************************
 *
 *	Video Graphics Adapter BIOS subfunctions
 *
 *****************************************************************************/

// VGA_DAC

#define VGA_PAGING_MODE				0x00
#define VGA_ACTIVE_PAGE				0x01
#define VGA_4_PAGES_OF_64_COLORS		0x00	// Separate palette into 4 blocks of 64 colors 
#define VGA_16_PAGES_OF_16_COLORS		0x01	// Separate palette into 16 blocks of 16 colors 

#define VGA_SET_PALETTE_COLOR			0x00	// Write to the DAC 
#define VGA_SET_OVERSCAN_COLOR			0x01	// Write border color register 
#define VGA_SET_ENTIRE_PALETTE_AND_OVERSCAN	0x02	// Write entire DAC and border color 
#define VGA_MODE_CONTROL			0x03	// Sets the blink/intensity field 
#define VGA_GET_PALETTE_COLOR			0x07	// Read from the DAC 
#define VGA_GET_OVERSCAN_COLOR			0x08	// Read the border color register 
#define VGA_GET_ENTIRE_PALETTE_AND_OVERSCAN	0x09	// Read entire DAC and border color 
#define VGA_SET_PALETTE				0x10	// Write to individual DAC register 
#define VGA_SET_PALETTE_RANGE			0x12	// Write to a range of DAC registers 
#define VGA_SET_COLOR_PAGE			0x13	// Divides DAC and makes a block active 
#define VGA_GET_PALETTE				0x15	// Read from individual DAC register 
#define VGA_GET_PALETTE_RANGE			0x17	// Read from a range of DAC registers 
#define VGA_SET_PEL_MASK			0x18	// ? 
#define VGA_GET_PEL_MASK			0x19	// ? 
#define VGA_GET_COLOR_PAGE			0x1a	// Returns the 256 color register paging mode 
#define VGA_GRAY_SCALE_PALETTE			0x1b	// Converts DAC block registers to gradient 

// VGA_CHAR_GENERATOR

#define VGA_LOAD_USER_FONT			0x00	// Load font into 1 of 8 memory areas without recalculating mode parameters 
#define VGA_LOAD_8x14_FONT			0x01	// Load 8x14 font without recalculating mode parameters 
#define VGA_LOAD_8x8_FONT			0x02	// Load 8x8 font without recalculating mode parameters 
#define VGA_SET_FONT_BLOCK_SPECIFIER		0x03	// Activate 1 of 8 character sets 
#define VGA_LOAD_8x16_FONT			0x04	// Load 8x16 font without recalculating mode parameters 
#define VGA_LOAD_USER_FONT_CRT			0x10	// Load font into 1 of 8 memory areas and recalculates mode parameters 
#define VGA_8x14_FONT_CRT			0x11	// Load 8x14 font and recalculate mode parameters 
#define VGA_8x8_FONT_CRT			0x12	// Load 8x8 font and recalculate mode parameters 
#define VGA_8x16_FONT_CRT			0x14	// Load 8x16 font and recalculate mode parameters 
#define VGA_GET_FONT_INFO			0x30	// Read a pointer to a font character definition table 

#define VGA_INT_1Fh_POINTER			0x00
#define VGA_INT_44h_POINTER			0x01
#define VGA_8x14_FONT_POINTER			0x02
#define VGA_8x8_FONT_POINTER			0x03	// Characters 0-127 
#define VGA_8x8_EXTENDED_FONT_POINTER		0x04	// Characters 128-255 
#define VGA_9x14_FONT_POINTER			0x05
#define VGA_8x16_FONT_POINTER			0x06
#define VGA_9x16_FONT_POINTER			0x07

// VGA_ALTERNATE_SELECT

#define VGA_INTENSITY				0x00
#define VGA_BLINK				0x01
#define VGA_200_SCAN_LINES			0x00
#define VGA_350_SCAN_LINES			0x01
#define VGA_400_SCAN_LINES			0x02

#define VGA_GET_INFO				0x10	// Read status of VGA configuration 
#define VGA_SET_PRINT_SCREEN_ROUTINE		0x20	// Select alternate print screen routine 
#define VGA_SET_TEXT_MODE_SCAN_LINES		0x30	// Set number of scan lines for text mode 
#define VGA_DEFAULT_PALETTE_LOADING		0x31	// Enable/Disable loading of default palette 
#define VGA_VIDEO_ADAPTER			0x32	// Enable/Disable the video adapter 
#define VGA_GRAY_SCALE_SUMMING			0x33	// Enable/Disable grayscale summing 
#define VGA_CURSOR_EMULATION			0x34	// Enable/Disable cursor emulation 
#define VGA_SWITCH_ACTIVE_DISPLAY		0x35	// Allow selection between 1 of 2 adapters 
#define VGA_VIDEO_SCREEN			0x36	// Enable/Disable video refresh on display devices 

// VGA_DISPLAY_COMBINATION_CODE

#define VGA_GET					0x00
#define VGA_SET					0x01

// VGA_VIDEO_STATE

#define	VGA_STATE_HARDWARE			0x01
#define VGA_STATE_BIOS_DATA			0x02
#define VGA_STATE_DAC				0x04
#define VGA_STATE_MEMORY_MAP			0x0d
#define VGA_STATE_REGISTERS			0x0f

#define VGA_QUERY				0x00	// Get minimum buffer size required to store state 
#define VGA_SAVE				0x01	// Loads buffer with requested state data 
#define VGA_RESTORE				0x02	// Restores previously saved state from buffer 

// VGA_GET_STATE_INFO

typedef struct
{
	line StaticTablePointer;
	byte CurrentMode;
	word nCharColumns;
	word BytesPerDisplayPage;
	word OffsetOfCurrentPage;
	byte CursorPositions[8][2];
	byte CursorStartLine;
	byte CursorEndLine;
	byte ActiveDisplayPage;
	word CRTC;
	byte ModeRegister;
	byte ColorRegister;
	byte nRows;
	byte CharHeight;
	byte ActiveDCC;
	byte InactiveDCC;
	word nColors;
	word nPages;
	byte nScanLines;
	byte PrimaryCharGenerator;
	byte SecondaryCharGenerator;
	byte StateInfo;
	byte Reserved0[3];
	byte TotalMemory;
	byte SavePointerState;
	byte Reserved1[13];

} VGAstate;

typedef struct
{
	byte Modes[3];
	line Reserved0;
	byte AvailableScanLines;
	byte AvailableCharBlocks;
	byte nActiveBlocks;
	byte Functions[2];
	word Reserved1;
	byte SavePointerFunction;
	byte Reserved2;

} VGAstatic;

// VGAstate.nScanLines

// #define VGA_200_SCAN_LINES			0x00
// #define VGA_350_SCAN_LINES			0x01
// #define VGA_400_SCAN_LINES			0x02
#define VGA_480_SCAN_LINES			0x03

// VGAstate.StateInfo

#define VGA_ALL_MODES_ON_ALL_MONITORS		0x01
#define VGA_GRAY_SCALE_SUM			0x02
#define VGA_MONOCHROME_DISPLAY			0x04
#define VGA_DEFAULT_PALETTE_INIT		0x08
#define VGA_CGA_CURSOR_EMULATION		0x10
#define VGA_BLINKING				0x20

// VGAstate.TotalMemory

#define VGA_64K					0x00
#define VGA_128K				0x01
#define VGA_192K				0x02
#define VGA_256K				0x03

// VGAstate.SavePointerState

#define VGA_512_CHAR_SET			0x01
#define VGA_DYNAMIC_SAVE_AREA			0x02
#define VGA_ALPHA_FONT_OVERRIDE			0x04
#define VGA_GRAPHICS_FONT_OVERRIDE		0x08
#define VGA_PALETTE_OVERRIDE			0x10
#define VGA_DCC_EXTENSION			0x20

// VGAstatic.AvailableScanLines

// #define VGA_200_SCAN_LINES			0x00
// #define VGA_350_SCAN_LINES			0x01
// #define VGA_400_SCAN_LINES			0x02

// VGAstatic.MiscFunctions[0]

// #define VGA_ALL_MODES_ON_ALL_MONITORS	0x01
// #define VGA_GRAY_SCALE_SUM			0x02
#define VGA_CHAR_FONT_LOADING			0x04
#define VGA_DEFAULT_PALETTE_LOADED		0x08
#define VGA_CURSOR_EMULATION_ENABLED		0x10
#define VGA_EGA_PALETTE				0x20
#define VGA_COLOR_REGISTER_LOADING		0x40
#define VGA_COLOR_REGISTER_PAGING_MODE_SELECT	0x80

// VGAstatic.MiscFunctions[1]

#define VGA_LIGHT_PEN				0x01
#define VGA_SAVE_RESTORE_VIDEO_STATE		0x02
#define VGA_BLINKING_CONTROL			0x04
#define VGA_DCC_CONTROL				0x08

// VGAstatic.SavePointerFunction

// #define VGA_512_CHAR_SET			0x01
// #define VGA_DYNAMIC_SAVE_AREA		0x02
// #define VGA_ALPHA_FONT_OVERRIDE		0x04
// #define VGA_GRAPHICS_FONT_OVERRIDE		0x08
// #define VGA_PALETTE_OVERRIDE			0x10
// #define VGA_DDC_EXTENSION			0x20

/******************************************************************************
 *
 *	Video Graphics Adapter functions using BIOS
 *
 *****************************************************************************/

typedef enum
{
	VGA_COLOR_MODE,
	VGA_MONOCHROME_MODE,
	VGA_FEATURE_BITS,
	VGA_SWITCH_SETTING,
	VGA_INACTIVE,
	VGA_ACTIVE

} VGAquery;

// vga functions

void vgaSetMode(int mode);

void vgaSetCursorType(int start, int end);

void vgaSetCursorPosition(int page, int row, int column);

void vgaGetCursorPosition(int page, int *row, int *column, int *start, int *end);

void vgaSetActiveDisplayPage(int page);

void vgaScrollActivePageUp(int lines, int clearing, int topRow, int topColumn, int bottomRow, int bottomColumn);

void vgaScrollActivePageDown(int lines, int clearing, int topRow, int topColumn, int bottomRow, int bottomColumn);

void vgaGetAttributeAtCursor(int page, char *ascii, int *attribute);

void vgaSetAttributeAtCursor(int page, char ascii, int attribute, int repeat);

void vgaWriteCharAtCursor(int page, char ascii, char color, int repeat);

void vgaSetPixel(int page, int x, int y, char color);

char vgaGetPixel(int page, int x, int y);

void vgaWriteTeletypeChar(int page, char ascii, char color);

void vgaGetCurrentVideoState(int *page, int *mode, int *columns);

// DAC functions

void vgaSetPaletteColor(int index, char color);

void vgaSetOverscanColor(char color);

void vgaSetEntirePaletteAndOverscan(char *buffer);

void vgaModeControl(int state);

char vgaGetPaletteColor(int index);

char vgaGetOverscanColor();

void vgaGetEntirePaletteAndOverscan(char *buffer);

void vgaSetPalette(int index, char red, char green, char blue);

void vgaSetPaletteRange(int index, int count, char palette[256][3]);

void vgaSetColorPage(int function, int page);

void vgaGetPalette(int index, char *red, char *green, char *blue);

void vgaGetPaletteRange(int index, int count, char palette[256][3]);

int vgaGetColorPage(int function);

void vgaGrayScaleSumming(int index, int count);

// Character generator functions

void vgaLoadUserFont(int buffer, int nChar, int offset, int bytesPerChar, void *table);

void vgaLoadROMFont(int buffer, int font);

int vgaGetFontInfo(int font, void *buffer);

// Alternate select functions

int vgaGetInfo(VGAreturns result);

BOOL vgaSetTextModeScanLines(int scanlines);

BOOL vgaStateOption(int option, int state);

BOOL vgaSwitchActiveDisplay(int function, void *buffer);

// Write string

void vgaWriteString(int page, int flags, char color, int row, int column, const char *string, int length);

// Display Combination Code

int vgaGetDCC(VGAreturns status);

BOOL vgaSetDCC(int active, int inactive);

// State information

BOOL vgaGrabState(VGAstate *state);

// Video state

int vgaQueryStateBufferSize(int flags);

BOOL vgaSaveState(int flags, void *buffer, int length);

BOOL vgaRestoreState(int flags, void *buffer, int length);


// Use default C data alignment from now on
#pragma pack()


#endif // VGA_h 
