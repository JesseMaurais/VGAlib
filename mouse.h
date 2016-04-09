
/*******************************************************************************
 *
 *	Mouse Cursor library
 *
 *	Additional information provided by Roger Morgan
 *	http://www.htl-steyr.ac.at/~morg/pcinfo/hardware/interrupts/inte1at0.htm
 *	Additional information provided by Ahmad Hafiz
 *	http://www.geocities.com/SiliconValley/Vista/2459/programming/mouse.htm
 */

#ifndef mouse_h
#define mouse_h

/*******************************************************************************
 *
 *	Mouse Cursor BIOS functions
 *
 ******************************************************************************/

#define MC_MOUSE_INT86				0x33	/* Mouse BIOS interrupt number */

#define MC_HORIZONTAL				640	/* Cursor always maps horizontal from 0-639 */
#define MC_VERTICAL				200	/* Cursor always maps vertical from 0-199 */

#define MC_INITIALIZE				0x00	/* Load the mouse driver */
#define MC_SHOW					0x01	/* Makes cursor visible */
#define MC_HIDE					0x02	/* Makes cursor invisible */
#define MC_GET_STATUS				0x03
#define MC_SET_CURSOR				0x04
#define MC_GET_PRESSED				0x05
#define MC_GET_RELEASED				0x06
#define MC_LIMIT_HORIZONTAL			0x07	/* Limit horizontal cursor movement */
#define MC_LIMIT_VERTICAL			0x08	/* Limit vertical cursor movement */
#define MC_SET_CURSOR_GRAPHIC			0x09
#define MC_SET_TEXT_CURSOR			0x0a
#define MC_GET_MICKEYS				0x0b
#define MC_USER_INTERRUPT_SUBROUTINE		0x0c
#define MC_LIGHT_PEN_EMULATION_ON		0x0d
#define MC_LIGHT_PEN_EMULATION_OFF		0x0e
#define MC_MICKEY_PIXEL_RATIO			0x0f
#define MC_HIDDEN_REGION			0x10
#define MC_DOUBLE_SPEED_THRESHOLD		0x13
#define MC_SWAP_INTERRUPT_SUBROUTINES		0x14
#define MC_GET_STATE_BUFFER_SIZE		0x15
#define MC_SAVE_STATE				0x16
#define MC_RESTORE_STATE			0x17
#define MC_SET_USER_INTERRUPT_ADDRESS		0x18
#define MC_GET_USER_INTERRUPT_ADDRESS		0x19
#define MC_SET_SENSITIVITY			0x1a
#define MC_GET_SENSITIVITY			0x1b
#define MC_INTERRUPT_RATE			0x1c
#define MC_SET_DISPLAY_PAGE			0x1d
#define MC_GET_DISPLAY_PAGE			0x1e
#define MC_DISABLE				0x1f
#define MC_ENABLE				0x20
#define MC_RESET				0x21
#define MC_SET_LANGUAGE				0x22
#define MC_GET_LANGUAGE				0x23
#define MC_GET_INFO				0x24

/* MC_INITIALIZE */

#define MC_INSTALLED				0xffff

/* MC_GET_STATUS */

#define MC_NONE					0x0
#define MC_LEFT					0x1
#define MC_RIGHT				0x2
#define MC_MIDDLE				0x3

/* MC_SET_TEXT_CURSOR */

#define MC_SOFTWARE_CURSOR			0x0
#define MC_HARDWARE_CURSOR			0x1

/* MC_USER_INTERRUPT_SUBROUTINE */

#define MC_CURSOR_POSITION_CHANGED		0x01
#define MC_LEFT_BUTTON_PRESSED			0x02
#define MC_LEFT_BUTTON_RELEASED			0x04
#define MC_RIGHT_BUTTON_PRESSED			0x08
#define MC_RIGHT_BUTTON_RELEASED		0x10

/* MC_INTERRUPT_RATE */

#define MC_30_PER_SECOND			0x1
#define MC_50_PER_SECOND			0x2
#define MC_100_PER_SECOND			0x3
#define MC_200_PER_SECOND			0x4

/* MC_{GET|SET}_LANGUAGE */

#define MC_ENGLISH				0x0
#define MC_FRENCH				0x1
#define MC_DUTCH				0x2
#define MC_GERMAN				0x3
#define MC_SWEDISH				0x4
#define MC_FINNISH				0x5
#define MC_SPANISH				0x6
#define MC_PORTUGUESE				0x7
#define MC_ITALIAN				0x8

/* MC_GET_INFO */

#define MC_BUS					0x1
#define MC_SERIAL				0x2
#define MC_INPORT				0x3
#define MC_PS2					0x4
#define MC_HEWLETT_PACKARD			0x5

/*******************************************************************************
 *
 *	Mouse Cursor functions using BIOS
 *
 ******************************************************************************/

BOOL mcInitialize();

void mcShow();

void mcHide();

int mcGetStatus(int *x, int *y);

void mcSetCursor(int x, int y);

int mcGetPressed(int button, int *count, int *x, int *y);

int mcGetReleased(int button, int *count, int *x, int *y);

void mcLimitHorizontal(int min, int max);

void mcLimitVertical(int min, int max);

void mcSetCursorGraphic(int xHotSpot, int yHotSpot, char bitmap[2][8][4]);

void mcSetTextCursor(int type, int start, int end);

void mcGetMickeys(int *horizontal, int *vertical);

void mcUserInterruptSubroutine(long address, int mask);

void mcLightPenEmulationOn();

void mcLightPenEmulationOff();

void mcMickeyPixelRatio(int horizontal, int vertical);

void mcHiddenRegion(int xMin, int yMin, int xMax, int yMax);

void mcDoubleSpeedThreshold(int mickeys);

int mcGetStateBufferSize();

void mcSaveState(void *buffer, int size);

void mcRestoreState(void *buffer, int size);

void mcSetSensitivity(int horizontal, int vertical, int threshold);

void mcGetSensitivity(int *horizontal, int *vertical, int *threshold);

void mcInterruptRate(int code);

void mcSetDisplayPage(int page);

int mcGetDisplayPage();

void mcSetLanguage(int language);

int mcGetLanguage();

void mcGetInfo(short *version, int *type, int *irq);


#endif /* mouse_h */
