
/*******************************************************************************
 *
 *	Mouse Cursor library
 *
 *	Additional information provided by Roger Morgan
 *	http://www.htl-steyr.ac.at/~morg/pcinfo/hardware/interrupts/inte1at0.htm
 *	Additional information provided by Ahmad Hafiz
 *	http://www.geocities.com/SiliconValley/Vista/2459/programming/mouse.htm
 */

#include "intern.h"
#include "farseg.h"
#include "mouse.h"

/*******************************************************************************
 *
 *
 *	Mouse Cursor functions using BIOS
 *
 *
 ******************************************************************************/

void mcFunction(int function, __dpmi_regs *regs)
{
	regs->x.ax = function;
	__dpmi_int(MC_MOUSE_INT86, regs);
}

/*******************************************************************************
 *
 *	MC_INITIALIZE (00h)
 *
 *	Resets mouse to default values, positioning the cursor at center of the
 *	screen and hidding it. Custom interrupts are disabled. Double speed
 *	threshold is set to 64 mickeys per second, horizontal mickey-to-pixel
 *	ratio is 8 to 8, vertical mickey-to-pixel ratio 16 to 8.
 *
 ******************************************************************************/

BOOL mcInitialize()
{
	__dpmi_regs regs;
	mcFunction(MC_INITIALIZE, &regs);
	return regs.x.ax == MC_INSTALLED;
}

/*******************************************************************************
 *
 *	MC_SHOW (01h)
 *
 *	When first initialized, and everytime the driver is reset, the cursor
 *	bitmap on the graphics display is hidden. A BIOS flag must be set so the
 *	cursor will be drawn.
 *
 ******************************************************************************/

void mcShow()
{
	__dpmi_regs regs;
	mcFunction(MC_SHOW, &regs);
}

/*******************************************************************************
 *
 *	MC_HIDE (02h)
 *
 *	At certain times it is necessary to hide the cursor, like when we are
 *	reading a pixel value from video memory.
 *
 ******************************************************************************/

void mcHide()
{
	__dpmi_regs regs;
	mcFunction(MC_HIDE, &regs);
}

/*******************************************************************************
 *
 *	MC_GET_STATUS (03h)
 *
 *	Returns basic information about the mouse state, including which buttons
 *	are being held down, and the current cordinates of the cursor hot-spot.
 *	Note that the cursor recognizes a uniform coordinate set regardless of
 *	the current video settings, in which the hozizontal spans from 0 to 639
 *	and the vertical from 0 to 199.
 *
 ******************************************************************************/

int mcGetStatus(int *x, int *y)
{
	__dpmi_regs regs;
	mcFunction(MC_GET_STATUS, &regs);
	setsafe(x, regs.x.cx);
	setsafe(y, regs.x.dx);
	return regs.x.bx;
}

/*******************************************************************************
 *
 *	MC_SET_CURSOR (04h)
 *
 *	Warps the cursor to a given screen coordinate with the uniform system
 *	described above.
 *
 ******************************************************************************/

void mcSetCursor(int x, int y)
{
	__dpmi_regs regs;
	regs.x.cx = x;
	regs.x.dx = y;
	mcFunction(MC_SET_CURSOR, &regs);
}

/*******************************************************************************
 *
 *	MC_GET_PRESSED (05h)
 *
 *	Obtains information about a specific button. How many times it had been
 *	pressed since this function was last called, the screen coordinates of
 *	the cursor location on the last press. Also returns status of all the
 *	buttons.
 *
 ******************************************************************************/

int mcGetPressed(int button, int *count, int *x, int *y)
{
	__dpmi_regs regs;
	regs.x.bx = button;
	mcFunction(MC_GET_PRESSED, &regs);
	setsafe(count, regs.x.bx);
	setsafe(x,     regs.x.cx);
	setsafe(y,     regs.x.dx);
	return regs.x.ax;
}

/*******************************************************************************
 *
 *	MC_GET_RELEASED (06h)
 *
 *	Obtains information about a specific button. How many times it had been
 *	released since the function was last called, the screen coordinates of
 *	the cursor location on the last release. Also returns status of all the
 *	buttons.
 *
 ******************************************************************************/

int mcGetReleased(int button, int *count, int *x, int *y)
{
	__dpmi_regs regs;
	regs.x.bx = button;
	mcFunction(MC_GET_RELEASED, &regs);
	setsafe(count, regs.x.bx);
	setsafe(x,     regs.x.cx);
	setsafe(y,     regs.x.dx);
	return regs.x.ax;
}

/*******************************************************************************
 *
 *	MC_LIMIT_HORIZONTAL (07h)
 *
 *	Restricts cursor horizontal movement to a specified subsection of the
 *	screen. If the minimum value is greater than the maximum value they are
 *	swapped before being applied.
 *
 ******************************************************************************/

void mcLimitHorizontal(int minimum, int maximum)
{
	__dpmi_regs regs;
	regs.x.cx = minimum;
	regs.x.dx = maximum;
	mcFunction(MC_LIMIT_HORIZONTAL, &regs);
}

/*******************************************************************************
 *
 *	MC_LIMIT_VERTICAL (08h)
 *
 *	Restricts cursor vertical movement to a specified subsection of the
 *	screen. If the minimum value is greater than the maximum value they are
 *	swapped before being applied.
 *
 ******************************************************************************/

void mcLimitVertical(int minimum, int maximum)
{
	__dpmi_regs regs;
	regs.x.cx = minimum;
	regs.x.dx = maximum;
	mcFunction(MC_LIMIT_VERTICAL, &regs);
}

/*******************************************************************************
 *
 *	MC_SET_CURSOR_GRAPHIC (09h)
 *
 *	Allows user to set a custom bitmap that the device will draw instead of
 *	the default "arrow" pointer. The x and y hot-spot represent the cursor's
 *	actual screen coordinate, whereas in this definition they are provided
 *	local to the bitmap. The bitmap itself is actually two bitmaps. First is
 *	the screen mask bitmap which is AND'ed to the screen, and the second is
 *	the cursor mask bitmap which is XOR'ed to the screen, so that the cursor
 *	can mask itself and not wipe out image data already written to video
 *	memory.
 *
 ******************************************************************************/

void mcSetCursorGraphic(int xHotSpot, int yHotSpot, char bitmap[2][8][4])
{
	__dpmi_regs regs;
	regs.x.bx = xHotSpot;
	regs.x.cx = yHotSpot;
	/* Copy data from bitmap into transfer buffer */
	dosmemput(bitmap, 64, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	mcFunction(MC_SET_CURSOR_GRAPHIC, &regs);
}

/*******************************************************************************
 *
 *	MC_SET_TEXT_CURSOR (0ah)
 *
 ******************************************************************************/

void mcSetTextCursor(int type, int start, int end)
{
	__dpmi_regs regs;
	regs.x.bx = type;
	regs.x.cx = start;
	regs.x.dx = end;
	mcFunction(MC_SET_TEXT_CURSOR, &regs);
}

/*******************************************************************************
 *
 *	MC_GET_MICKEYS (0bh)
 *
 ******************************************************************************/

void mcGetMickeys(int *horizontal, int *vertical)
{
	__dpmi_regs regs;
	mcFunction(MC_GET_MICKEYS, &regs);
	setsafe(horizontal, regs.x.cx);
	setsafe(vertical,   regs.x.dx);
}

/*******************************************************************************
 *
 *	MC_USER_INTERRUPT_SUBROUTINE (0ch)
 *
 ******************************************************************************/

void mcUserInterruptSubroutine(long address, int mask)
{
	__dpmi_regs regs;
	regs.x.es = far2seg(address);
	regs.x.dx = far2off(address);
	regs.x.cx = mask;
	mcFunction(MC_USER_INTERRUPT_SUBROUTINE, &regs);
}

/*******************************************************************************
 *
 *	MC_LIGHT_PEN_EMULATION_ON  (0dh)
 *	MC_LIGHT_PEN_EMULATION_OFF (0eh)
 *
 *	Toggle light pen emulation. The light pen is considered down when both
 *	buttons are down, or off palette if both buttons are up.
 *
 ******************************************************************************/

void mcLightPenEmulationOn()
{
	__dpmi_regs regs;
	mcFunction(MC_LIGHT_PEN_EMULATION_ON, &regs);
}

void mcLightPenEmulationOff()
{
	__dpmi_regs regs;
	mcFunction(MC_LIGHT_PEN_EMULATION_OFF, &regs);
}

/*******************************************************************************
 *
 *	MC_MICKEY_PIXEL_RATIO (0fh)
 *
 *	Sets the ratio between physical cursor movement (mickeys) and screen
 *	coordinate changes (pixels).
 *
 ******************************************************************************/

void mcMickeyPixelRatio(int horizontal, int vertical)
{
	__dpmi_regs regs;
	regs.x.cx = horizontal;
	regs.x.dx = vertical;
	mcFunction(MC_MICKEY_PIXEL_RATIO, &regs);
}

/*******************************************************************************
 *
 *	MC_HIDDEN_REGION (10h)
 *
 *	Defines an area of screen in which the mouse is not visible even if the
 *	visiblity flag was already set using MC_SHOW. Successive calls to the
 *	MC_SHOW function will remove the hidden region.
 *
 ******************************************************************************/

void mcHiddenRegion(int xMin, int yMin, int xMax, int yMax)
{
	__dpmi_regs regs;
	regs.x.cx = xMin;
	regs.x.dx = yMin;
	regs.x.si = xMax;
	regs.x.di = yMax;
	mcFunction(MC_HIDDEN_REGION, &regs);
}

/*******************************************************************************
 *
 *	MC_DOUBLE_SPEED_THRESHOLD (13h)
 *
 *	Cursor speed is doubled when the cursor moves across the screen at the
 *	threshold speed.
 *
 ******************************************************************************/

void mcDoubleSpeedThreshold(int mickeys)
{
	__dpmi_regs regs;
	regs.x.dx = mickeys;
	mcFunction(MC_DOUBLE_SPEED_THRESHOLD, &regs);
}

/*******************************************************************************
 *
 *	MC_SWAP_INTERRUPT_SUBROUTINES (14h)
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	MC_GET_STATE_BUFFER_SIZE (15h)
 *
 *	Used before functions MC_SAVE_STATE and MC_RETORE_STATE to determine the
 *	amount of memory needed to save the mouse state before giving up control
 *	of it to another program.
 *
 ******************************************************************************/

int mcGetStateBufferSize()
{
	__dpmi_regs regs;
	mcFunction(MC_GET_STATE_BUFFER_SIZE, &regs);
	return regs.x.bx;
}

/*******************************************************************************
 *
 *	MC_SAVE_STATE (16h)
 *
 *	Used to save mouse state information before relinquishing control to
 *	another programs mouse handler.
 *
 ******************************************************************************/

void mcSaveState(void *buffer, int size)
{
	__dpmi_regs regs;
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	mcFunction(MC_SAVE_STATE, &regs);
	/* Read state information into buffer */
	dosmemget(__tb, size, buffer);
}

/*******************************************************************************
 *
 *	MC_RESTORE_STATE (17h)
 *
 *	Used to restore mouse state information after regaining control from
 *	another programs mouse handler.
 *
 ******************************************************************************/

void mcRestoreState(void *buffer, int size)
{
	__dpmi_regs regs;
	/* Copy state data into transfer buffer */
	dosmemput(buffer, size, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.dx = far2off(__tb);
	mcFunction(MC_RESTORE_STATE, &regs);
}

/*******************************************************************************
 *
 *	MC_SET_INTERRUPT_ADDRESS (18h)
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	MC_GET_INTERRUPT_ADDRESS (19h)
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	MC_SET_SENSITIVITY (1ah)
 *	MC_GET_SENSITIVITY (1bh)
 *
 *	Sets/Gets the mouse sensitivity by setting the ratio between mickeys and
 *	screen pixels. Basically the same as using MC_MICKEY_PIXEL_RATIO and
 *	MC_DOUBLE_SPEED_THRESHOLD together.
 *
 ******************************************************************************/

void mcSetSensitivity(int horizontal, int vertical, int threshold)
{
	__dpmi_regs regs;
	regs.x.bx = horizontal;
	regs.x.cx = vertical;
	regs.x.dx = threshold;
	mcFunction(MC_SET_SENSITIVITY, &regs);
}

void mcGetSensitivity(int *horizontal, int *vertical, int *threshold)
{
	__dpmi_regs regs;
	mcFunction(MC_GET_SENSITIVITY, &regs);
	setsafe(horizontal, regs.x.bx);
	setsafe(vertical,   regs.x.cx);
	setsafe(threshold,  regs.x.dx);
}

/*******************************************************************************
 *
 *	MC_INTERRUPT_RATE (1ch)
 *
 *	Works with MC_INPORT mice only. Sets the rate at which the mouse status
 *	is polled by the driver. Faster rates provide a better resolution but
 *	eat more CPU time.
 *
 ******************************************************************************/

void mcInterruptRate(int code)
{
	__dpmi_regs regs;
	regs.x.bx = code;
	mcFunction(MC_INTERRUPT_RATE, &regs);
}

/*******************************************************************************
 *
 *	MC_SET_DISPLAY_PAGE (1dh)
 *
 *	Sets the CRT page on which the mouse cursor is to be displayed.
 *
 ******************************************************************************/

void mcSetDisplayPage(int page)
{
	__dpmi_regs regs;
	regs.x.bx = page;
	mcFunction(MC_SET_DISPLAY_PAGE, &regs);
}

/*******************************************************************************
 *
 *	MC_GET_DISPLAY_PAGE (1eh)
 *
 *	Gets the CRT page on which the cursor is being displayed.
 *
 ******************************************************************************/

int mcGetDisplayPage()
{
	__dpmi_regs regs;
	mcFunction(MC_GET_DISPLAY_PAGE, &regs);
	return regs.x.bx;
}

/*******************************************************************************
 *
 *	MC_DISABLE (1fh)
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	MC_ENABLE (20h)
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	MC_RESET (21h)
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	MC_SET_LANGUAGE (22h)
 *
 *	Only works with international version of the mouse driver.
 *
 ******************************************************************************/

void mcSetLanguage(int language)
{
	__dpmi_regs regs;
	regs.x.bx = language;
	mcFunction(MC_SET_LANGUAGE, &regs);
}

/*******************************************************************************
 *
 *	MC_GET_LANGUAGE (23h)
 *
 *	Only works with international version of the mouse driver.
 *
 ******************************************************************************/

int mcGetLanguage()
{
	__dpmi_regs regs;
	mcFunction(MC_GET_LANGUAGE, &regs);
	return regs.x.bx;
}

/*******************************************************************************
 *
 *	MC_GET_INFO (24h)
 *
 *	Returns basic information about the mouse driver.
 *
 ******************************************************************************/

void mcGetInfo(short *version, int *type, int *irq)
{
	__dpmi_regs regs;
	mcFunction(MC_GET_INFO, &regs);
	setsafe(version, regs.x.bx);
	setsafe(type,    regs.h.ch);
	setsafe(irq,     regs.h.cl);
}
