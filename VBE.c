/*******************************************************************************
 *
 *	VBE 3.0 library
 *
 *	Based on the official specification
 *	http://www.vesa.org/public/VBE/vbe3.pdf
 *	Some specification from Intel
 *	http://www.msc-ge.com/download/pc-system/ipc-produkte/pisa/VGA%20BIOS%20CT%2069030%20Ref-Guide.PDF
 *	Portions of code from Chris Walpole
 *	http://pages.cpsc.ucalgary.ca/~walpole/325/DJ_GRAPH.txt
 *	Portions of code from DJ Delorie
 *	http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
 *	http://www.delorie.com/djgpp/doc/ug/graphics/vbe20.html
 *	Additional information provided by MonsterSoft
 *	http://www.monstersoft.com/tutorial1/VESA_intro.html
 *	Additional information provided by Hugo Elias
 *	http://freespace.virgin.net/hugo.elias/graphics/x_svga.htm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "farseg.h"
#include "VGA.h"
#include "VBE.h"

/* Error reporting */

static char ErrorString[128] = "VESA no errors";

const char *vbeGetErrorString()
{
	return VESAErrorString;
}

/*
 * VBE_VESA_FUNCTION (4fh)
 *
 *	Makes a video interrupt call for a VESA function.
 *
 *	__dpmi_regs & regs
 *		Reference to a pseudo registers union to use for BIOS call.
 *	int function
 *		A flag indicating the VESA function to call.
 *			VBE_GET_INFO
 *			VBE_GET_MODE_INFO
 *			VBE_SET_MODE
 *			VBE_GET_MODE
 *			VBE_VIDEO_STATE
 *			VBE_BANK_SWITCH
 *			VBE_SCAN_LINE_LENGTH
 *			VBE_DISPLAY_START
 *			VBE_DAC_BITS
 *			VBE_DAC_DATA
 *			VBE_PROTECTED_MODE_INTERFACE
 *			VBE_PIXEL_CLOCK
 *	returns
 *		True if VESA BIOS call was successfully executed.
 */

BOOL vbeFunction(int function, __dpmi_regs *regs)
{
	/* VESA function to call */
	regs->h.ah = VBE_VESA_FUNCTION;
	regs->h.al = function;
	/* Video interrupt */
	__dpmi_int(VGA_VIDEO_INT86, regs);
	/* Check error flag */
	if(regs->h.al == VBE_FAILED)
	{
		/* Converts VBE constant to string name */
		const char *names[] =
		{
			"VBE_GET_INFO",
			"VBE_GET_MODE_INFO",
			"VBE_SET_MODE",
			"VBE_GET_MODE",
			"VBE_VIDEO_STATE",
			"VBE_BANK_SWITCH",
			"VBE_SCAN_LINE_LENGTH",
			"VBE_DISPLAY_START",
			"VBE_DAC_BITS",
			"VBE_DAC_DATA",
			"VBE_PROTECTED_MODE_INTERFACE",
			"VBE_PIXEL_CLOCK"
		};
		#define ERROR(msg) sprintf(ErrorString, msg, names[function]);
		/* Determine the nature of the error */
		switch(regs->h.ah)
		{
		case VBE_SUPPORTED:
		{
			ERROR("VESA BIOS failed execution of \"%s\" function");
			break;
		}
		case VBE_FAILED:
		{
			ERROR("VESA function \"%s\" failed to complete");
			break;
		}
		case VBE_FAILED_WITH_CONFIGURATION:
		{
			ERROR("VESA function \"%s\" is invalid with current configurations");
			break;
		}
		case VBE_FAILED_WITH_SETTINGS:
		{
			ERROR("VESA function \"%s\" is invalid with current video settings");
			break;
		}
		default:
		{
			ERROR("VESA function \"%s\" is not supported");
			break;
		}
		}
		#undef ERROR
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************
 *
 *	VBE_GET_INFO (00h)
 *
 *	Populates structure 'info' of type 'VBEinfo' with information about the
 *	video hardware structure. Data is transfered to __tb data segment before
 *	it is copied into the 'info' structure (which is pack aligned). The
 *	purpose of this function is to provide information to the calling
 *	program about the general capabilities of the Super VGA environment. The
 *	function fills an information block structure at the address specified
 *	by the caller. The information block size is 256 bytes.
 *
 ******************************************************************************/

/*
 * vbeGetInfo
 *
 *	VBEinfo * info
 *		A pointer to a structure in which information about the video
 *		hardware will be written.
 *	returns
 *		True if video BIOS supports the vbe2.0 extensions.
 */

BOOL vbeGetInfo(VBEinfo *pInfo, short *modes, char *oem, char *vendor, char *name)
{
	__dpmi_regs regs;
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, sizeof(VBEinfo), 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.di = far2off(__tb);
	if(vbeFunction(VBE_GET_INFO, &regs))
	{
		/* Copy data from the transfer buffer into VBEInfo structure */
		dosmemget(__tb, sizeof(VBEinfo), pInfo);
		/* Check the vbe signature */
		if(strncmp(pInfo->Signature, VBE_VESA_SIGNATURE, 4) == 0)
		{
			if(modes)
			{
				/* Copy video modes to local memory */
				nearmemcpyw(info->VideoModePtr, modes,  -1);
			}
			if(oem)
			{
				/* Copy OEM string into local memory */
				nearmemcpyb(info->OEMStringPtr, oem, '\0');
			}
			if(vendor)
			{
				/* Copy vendor name into local memory */
				nearmemcpyb(info->OEMVendorNamePtr, vendor, '\0');
			}
			if(name)
			{
				/* Copy product name into local memory */
				nearmemcpyb(info->OEMProductNamePtr, name, '\0');
			}
			return TRUE;
		}
	}
	return FALSE;
}

/*******************************************************************************
 *
 *	VBE_GET_MODE_INFO (01h)
 *
 *	Populates structure 'info' of type 'vbeModeInfo' with information about
 *	a video display mode. Data is transfered to the __tb data segment before
 *	it is copied into the 'info' structure (which is pack aligned). This
 *	function returns information about a specific Super VGA video mode that
 *	was provided by function 00h. The function fills a mode information
 *	structure at the address specified by the caller. The mode information
 *	block size is a maximum of 256 bytes. Some information provided by this
 *	function is implicitly defined by the VESA mode number. However, some
 *	Super VGA implementations might support other video modes than those
 *	defined by VESA. To provide access to these modes, this function also
 *	returns various other information about the mode.
 *
 ******************************************************************************/

/*
 * vbeGetModeInfo
 *
 *	vbeModeInfo * modeinfo
 *		A pointer to a structure in which information about a specific
 *		display mode will be written.
 *	int mode
 *		Flag indicating a display mode number supported by the hardware.
 *		A list of such flags are given in 'VBEInfo.VideoModePtr'
 *	returns
 *		True if the video mode is supported by the hardware.
 */

BOOL vbeGetModeInfo(VBEmodeInfo *pModeInfo, int mode)
{
	__dpmi_regs regs;
	regs.x.cx = mode;
	/* Initialize buffer to zero */
	farmemsetb(_dos_ds, __tb, sizeof(VBEmodeInfo), 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.di = far2off(__tb);
	if(vbeFunction(VBE_GET_MODE_INFO, &regs))
	{
		/* Copy data from the transfer buffer into vbeModeInfo structure */
		dosmemget(__tb, sizeof(VBEmodeInfo), pModeInfo);
		return TRUE;
	}
	return FALSE;
}

/*******************************************************************************
 *
 *	VBE_SET_MODE (02h)
 *	VBE_GET_MODE (03h)
 *
 *	Changes display settings given a video mode flag. Register BX holds the
 *	mode flag. You can tell it not to clear the screen if you want by OR'ing
 *	the 'VBE_NO_CLEAR' (8000h) flag, or to exclusively search for an LFB
 *	with the 'VBE_LINEAR_FRAME_BUFFER' (4000h) flag.
 *
 ******************************************************************************/

/*
 * vbeSetMode
 *
 *	int mode
 *		Flag indicating a display mode number supported by the hardware.
 *		A list of such flags are given in 'VBEInfo.VideoModePtr'
 *	vbeCRTCInfo * crtc
 *		An optional second paramter if 'mode' is OR'ed with the flag
 *		attribute 'VBE_CRTC'
 *	returns
 *		True if the display settings were successfully changed.
 */

BOOL vbeSetMode(int mode, VBEcrtcInfo *pCrtcInfo)
{
	__dpmi_regs regs;
	regs.x.bx = mode;
	/* Load the CRTC block if requested */
	if(pCrtcInfo)
	{
		/* Write CRTC info block to the transfer buffer */
		dosmemput(pCrtcInfo, sizeof(VBEcrtcInfo), __tb);
		/* Load the transfer buffer */
		regs.x.es = far2seg(__tb);
		regs.x.di = far2off(__tb);
	}
	if(vbeFunction(VBE_SET_MODE, &regs))
		return TRUE;
	else
		return FALSE;
}

/*
 * vbeGetMode
 *
 *	returns
 *		A mode flag indicating the current display settings or -1 if the
 *		function was unsuccessful.
 */

int vbeGetCurrentMode()
{
	__dpmi_regs regs;
	if(vbeFunction(VBE_GET_MODE, &regs))
		return regs.x.bx;
	else
		return -1;
}

/*******************************************************************************
 *
 *	VBE_VIDEO_STATE (04h)
 *
 *	These functions provide a mechanism to save and restore the Super VGA
 *	video state. The functions are a superset of the three subfunctions
 *	under standard VGA BIOS function 1Ch (save/restore video state). The
 *	complete Super VGA video state (except video memory) should be saveable
 *	or restoreable by setting the requested states mask (in the CX register)
 *	to 000Fh.
 *
 ******************************************************************************/

BOOL vbeVideoState(int subfunction, int flags, __dpmi_regs *regs)
{
	regs->h.dl = subfunction;
	regs->x.cx = flags;
	return vbeFunction(VBE_VIDEO_STATE, regs);
}

/*
 * vbeQueryStateBufferSize
 *
 *	int flags
 *		A set of OR'ed flags which indicate what parts of the video state
 *		are to be saved.
 *			VBE_STATE_HARDWARE
 *			VBE_STATE_BIOS_DATA
 *			VBE_STATE_DAC
 *			VBE_STATE_SVGA
 *	returns
 *		The size, in bytes, required to store state information. If the
 *		function was unsuccessful return is -1.
 */

int vbeQueryStateBufferSize(int flags)
{
	__dpmi_regs regs;
	if(vbeVideoState(VBE_QUERY, flags, &regs))
		/* Size is given in 64 byte blocks, but we want raw byte size */
		return regs.x.bx * 64;
	else
		return -1;
}

/*
 * vbeSaveState
 *
 *	int flags
 *		A set of OR'ed flags which indicate what parts of the video state
 *		are to be saved.
 *			VBE_STATE_HARDWARE
 *			VBE_STATE_BIOS_DATA
 *			VBE_STATE_DAC
 *			VBE_STATE_SVGA
 *	void * buffer
 *		A pointer to a block of memory where state information will be
 *		stored.
 *	int length
 *		The length of 'buffer' in bytes.
 *	returns
 *		True if state information was successfully saved.
 */

BOOL vbeSaveState(int flags, void *buffer, int length)
{
	__dpmi_regs regs;
	/* Initialize the buffer to zero. */
	farmemsetb(_dos_ds, __tb, length, 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.bx = far2off(__tb);
	if(vbeVideoState(VBE_SAVE, flags, &regs))
	{
		/* Copy data from the transfer buffer into allocated memory */
		dosmemget(__tb, length, buffer);
		return TRUE;
	}
	return FALSE;
}

/*
 * vbeRestoreState
 *
 *	int flags
 *		A set of OR'ed flags which indicate what parts of the video state
 *		are to be restored.
 *			VBE_STATE_HARDWARE
 *			VBE_STATE_BIOS_DATA
 *			VBE_STATE_DAC
 *			VBE_STATE_SVGA
 *	void * buffer
 *		A pointer to a block of memory where state information will be
 *		copied.
 *	int length
 *		The length of 'buffer' in bytes.
 *	returns
 *		True if state information was successfully restored.
 */

BOOL vbeRestoreState(int flags, void *buffer, int length)
{
	__dpmi_regs regs;
	/* Copy data from allocated memory into transfer buffer */
	dosmemput(buffer, length, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.bx = far2off(__tb);
	return vbeVideoState(VBE_RESTORE, flags, &regs);
}

/*******************************************************************************
 *
 * VBE_BANK_SWITCH (05h)
 *
 *	Switches the 64k window to a position indicated by the banking 'number'.
 *	This function sets or gets the position of the specified window in the
 *	video memory. Allows direct access to the hardware paging registers. To
 *	use this function properly, the software should use VESA BIOS function
 *	01h (Return Super VGA mode information) to determine the size, location,
 *	and granularity of the windows.
 *
 ******************************************************************************/

BOOL vbeBankSwitch(int subfunction, int window, __dpmi_regs *regs)
{
	regs->h.bh = subfunction;
	regs->h.bl = window;
	return vbeFunction(VBE_BANK_SWITCH, regs);
}

/*
 * vbeSetBankPosition
 *
 *	int window
 *		Indicates which window will be moved.
 *			VBE_WINDOW_A
 *			VBE_WINDOW_B
 *	int number
 *		The new position for the window bank.
 *	returns
 *		True if function was successfully executed.
 */

BOOL vbeSetBankPosition(int window, int number)
{
	__dpmi_regs regs;
	regs.x.dx = number;
	return vbeBankSwitch(VBE_SET, window , &regs);
}

/*
 * vbeGetBankPosition
 *
 *	int window
 *		Indicates which window will be polled.
 *			VBE_WINDOW_A
 *			VBE_WINDOW_B
 *	returns
 *		Position of the window bank, or -1 if function was unsuccessful.
 */

int vbeGetBankPosition(int window)
{
	__dpmi_regs regs;
	if(vbeBankSwitch(VBE_GET, window, &regs))
		return regs.x.dx;
	else
		return -1;
}

/*******************************************************************************
 *
 *	VBE_SCAN_LINE_LENGTH (06h)
 *
 *	Allows you to set up a logical video memory buffer that is wider than
 *	the displayed area of the screen. Function 07h then allows the
 *	application to set the starting position that is to be displayed.
 *
 ******************************************************************************/

BOOL vbeScanLineLength(int subfunction, __dpmi_regs *regs)
{
	regs->h.bl = subfunction;
	return vbeFunction(VBE_SCAN_LINE_LENGTH, regs);
}

/*
 * vbeSetScanLineLength
 *
 *	int length
 *		New length of the horizontal scan line in pixels.
 *	VBEreturns result
 *		What format the return value will be in.
 *			VBE_BYTES_PER_SCAN_LINE
 *			VBE_PIXELS_PER_SCAN_LINE
 *			VBE_NUMBER_OF_SCAN_LINES
 *			VBE_NONE
 *	returns
 *		Depends on the value of 'result' (see above) or -1 if function
 *		was unsuccessful (or 'result' was VBE_NONE).
 */

int vbeSetScanLineLength(int length, VBEreturns result)
{
	__dpmi_regs regs;
	regs.x.cx = length;
	if(vbeScanLineLength(VBE_SET, &regs))
	{
		switch(result)
		{
		case VBE_BYTES_PER_SCAN_LINE:
		{
			return regs.x.bx;
		}
		case VBE_PIXELS_PER_SCAN_LINE:
		{
			return regs.x.cx;
		}
		case VBE_NUMBER_OF_SCAN_LINES:
		{
			return regs.x.dx;
		}
		}
	}
	return -1;
}

/*
 * vbeGetScanLineLength
 *
 *	VBEreturns result
 *		What format the return value will be in.
 *			VBE_BYTES_PER_SCAN_LINE
 *			VBE_PIXELS_PER_SCAN_LINE
 *			VBE_NUMBER_OF_SCAN_LINES
 *	returns
 *		Depends on the value of 'result' (see above) or -1 if function
 *		was unsuccessful.
 */

int vbeGetScanLineLength(VBEreturns result)
{
	__dpmi_regs regs;
	if(vbeScanLineLength(VBE_GET, &regs))
	{
		switch(result)
		{
		case VBE_BYTES_PER_SCAN_LINE:
		{
			return regs.x.bx;
		}
		case VBE_PIXELS_PER_SCAN_LINE:
		{
			return regs.x.cx;
		}
		case VBE_NUMBER_OF_SCAN_LINES:
		{
			return regs.x.dx;
		}
		}
	}
	return -1;
}

/*******************************************************************************
 *
 *	VBE_DISPLAY_START (07h)
 *
 *	This function selects the pixel to be displayed in the upper left corner
 *	of the display from the logical page and can be used to pan and scroll
 *	around logical screens that are larger than the displayed screen. It can
 *	also be used to rapidly switch between two different displayed screens
 *	for double buffered animation effects.
 *
 ******************************************************************************/

BOOL vbeDisplayStart(int subfunction, __dpmi_regs *regs)
{
	regs->x.bx = subfunction;
	return vbeFunction(VBE_DISPLAY_START, regs);
}

/*
 * vbeSetDisplayStart
 *
 *	int x
 *		The left most pixel position in the virtual window for the
 *		visible display to be moved to.
 *	int y
 *		The upper most pixel position in the virtual window for the
 *		visible display to be moved to.
 *	returns
 *		True if visible display was moved in the virtual window.
 */

BOOL vbeSetDisplayStart(int x, int y)
{
	__dpmi_regs regs;
	regs.x.cx = x;
	regs.x.dx = y;
	return vbeDisplayStart(VBE_SET, &regs);
}

/*
 * vbeGetDisplayStart
 *
 *	int * x
 *		Will be written with the left most pixel position of the visible
 *		display.
 *	int * y
 *		Will be written with the upper most pixel position of the visible
 *		display.
 *	returns
 *		True if virtual window position was read correctly.
 */

BOOL vbeGetDisplayStart(int *x, int *y)
{
	__dpmi_regs regs;
	if(vbeDisplayStart(VBE_GET, &regs))
	{
		setsafe(x, regs.x.cx);
		setsafe(y, regs.x.dx);
		return TRUE;
	}
	return FALSE;
}

/*
 * vbeScheduleDisplayStartAddress
 *
 *	int address
 *		Display start address in bytes.
 *	returns
 *		True if visible display move was sheduled.
 */

BOOL vbeScheduleDisplayStartAddress(int address)
{
	__dpmi_regs regs;
	regs.d.ecx = address
	             return vbeDisplayStart(VBE_SET_SCHEDULE, &regs);
}

/*
 * vbeScheduleStereoDisplayStartAddress
 *
 *	int leftAddress
 *		Display start address of the left most stereo image in bytes.
 *	int rightAddress
 *		Display start address of the right most stereo image in bytes.
 *	returns
 *		True if visible display move was scheduled.
 */

BOOL vbeScheduleStereoDisplayStartAddress(int leftAddress, int rightAddress)
{
	__dpmi_regs regs;
	regs.d.ecx = leftAddress;
	regs.d.edx = rightAddress;
	return vbeDisplayStart(VBE_SET_SCHEDULE, &regs);
}

/*
 * vbeQueryScheduledStereoDisplayComplete
 *
 *	returns
 *		True if the scheduled display change was made, otherwise false.
 */

BOOL vbeQueryScheduledStereoDisplayComplete()
{
	__dpmi_regs regs;
	if(vbeDisplayStart(VBE_QUERY_SCHEDULE, &regs))
	{
		return regs.x.cx;
	}
	return FALSE;
}

/*
 * vbeEnableStereoMode
 *
 *	returns
 *		True if stereoscopic mode was enabled.
 */

BOOL vbeEnableStereoMode()
{
	__dpmi_regs regs;
	return vbeDisplayStart(VBE_ENABLE_STEREO_MODE, &regs);
}

/*
 * vbeDisableStereoMode
 *
 *	returns
 *		True if stereoscopic mode was disabled.
 */

BOOL vbeDisableStereoMode()
{
	__dpmi_regs regs;
	return vbeDisplayStart(VBE_DISABLE_STEREO_MODE, &regs);
}

/*
 * vbeSetDisplayStartOnSync
 *
 *	int x
 *		The left most pixel position in the virtual window for the
 *		visible display to be moved to.
 *	int y
 *		The upper most pixel position in the virtual window for the
 *		visible display to be moved to.
 *	returns
 *		True if visible display was moved in the virtual window.
 */

BOOL vbeSetDisplayStartOnSync(int x, int y)
{
	__dpmi_regs regs;
	regs.x.cx = x;
	regs.x.dx = y;
	return vbeDisplayStart(VBE_SET_SYNC, &regs);
}

/*
 * vbeSetDisplayStartAddressOnSync
 *
 *	int address
 *		Display start address in bytes.
 *	returns
 *		True if visible display move was sheduled.
 */

BOOL vbeSetDisplayStartAddressOnSync(int address)
{
	__dpmi_regs regs;
	regs.d.ecx = address
	             return vbeDisplayStart(VBE_SET_SYNC_ALTERNATE, &regs);
}

/*
 * vbeSetStereoDisplayStartOnSync
 *
 *	int x
 *		The left most pixel position in the virtual window for the
 *		visible display to be moved to.
 *	int y
 *		The upper most pixel position in the virtual window for the
 *		visible display to be moved to.
 *	returns
 *		True if visible display was moved in the virtual window.
 */

BOOL vbeSetStereoDisplayStartOnSync(int x, int y)
{
	__dpmi_regs regs;
	regs.x.cx = x;
	regs.x.dx = y;
	return vbeDisplayStart(VBE_SET_SYNC, &regs);
}

/*******************************************************************************
 *
 *	VBE_DAC_BITS (08h)
 *
 *	This function queries and selects the operating mode of the DAC palette.
 *	Some DACs are configurable to provide 6-bits, 8-bits, or more of color
 *	definition per red, green, and blue primary color. The DAC palette width
 *	is assumed to be reset to standard VGA 6-bits per primary during a
 *	standard or VESA mode (function 02h).
 *
 ******************************************************************************/

BOOL vbeDAC(int subfunction, __dpmi_regs *regs)
{
	regs->h.bl = subfunction;
	return vbeFunction(VBE_DAC_BITS, regs);
}

/*
 * vbeSetPaletteDepth
 *
 *	int bits
 *		The number of bits requested for color channels.
 *	returns
 *		The number of bits set after the function call, or -1 if the
 *		function failed.
 */
int vbeSetPaletteDepth(int bits)
{
	__dpmi_regs regs;
	regs.h.bh = bits;
	if(vbeDAC(VBE_SET, &regs))
		return regs.h.bh;
	else
		return -1;
}

/*
 * vbeGetPaletteDepth
 *
 *	returns
 *		The number of bits set, or -1 if the
 *		function failed.
 */
int vbeGetPaletteDepth()
{
	__dpmi_regs regs;
	if(vbeDAC(VBE_GET, &regs))
		return regs.h.bh;
	else
		return -1;
}

/*******************************************************************************
 *
 *	VBE_DAC_DATA (09h)
 *
 *	This function sets or gets the color registers in the RAMDAC. It is more
 *	important for RAMDAC's which are larger than a standard VGA RAMDAC. The
 *	standard interrupt 10h video BIOS color register interface functions
 *	assume standard VGA ports and VGA palette widths. This function offers a
 *	palette interface that is independant of the VGA assumptions.
 *
 ******************************************************************************/

/*
 * vbeSetPalette
 *
 *	int index
 *		The index of the first palette entry to be written.
 *	int count
 *		The number of palette entries that will be written.
 *	void * palette
 *		An array in sets of 3 bytes for each 'count' representing RGB
 *		values. RGB value range is dependant on VBE_DAC_BITS.
 *	returns
 *		True if the DAC was successfully written.
 */
BOOL vbeSetPalette(int index, int count, char palette[256][4])
{
	__dpmi_regs regs;
	regs.h.bl = VBE_SET;
	regs.x.cx = count * 4;
	regs.x.dx = index;
	/* Write palette data to the transfer buffer */
	dosmemput(palette[ index ], count * 4, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.di = far2off(__tb);
	return vbeFunction(VBE_DAC_DATA, &regs);
}

/*
 * vbeGetPalette
 *
 *	int index
 *		The index of the first palette entry to be read.
 *	int count
 *		The number of palette entries that will be read.
 *	void * palette
 *		An array in sets of 3 bytes for each 'count' representing RGB
 *		values. RGB value range is dependant on VBE_DAC_BITS.
 *	returns
 *		True if the DAC was successfully read.
 */
BOOL vbeGetPalette(int index, int count, char palette[256][4])
{
	__dpmi_regs regs;
	regs.h.bl = VBE_GET;
	regs.x.cx = count * 4;
	regs.x.dx = index;
	/* Initialize the buffer to zero */
	farmemsetb(_dos_ds, __tb, count * 4, 0);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.di = far2off(__tb);
	if(vbeFunction(VBE_DAC_DATA, &regs))
	{
		/* Copy data from the transfer buffer into palette */
		dosmemget(__tb, count * 4, palette[ index ]);
		return TRUE;
	}
	return FALSE;
}

/*
 * vbeSetPaletteOnSync
 *
 *	int index
 *		The index of the first palette entry to be written.
 *	int count
 *		The number of palette entries that will be written.
 *	void * palette
 *		An array in sets of 3 bytes for each 'count' representing RGB
 *		values. RGB value range is dependant on VBE_DAC_BITS.
 *	returns
 *		True if the DAC was successfully written.
 */
BOOL vbeSetPaletteOnSync(int index, int count, char palette[256][4])
{
	__dpmi_regs regs;
	regs.h.bl = VBE_SET_SYNC;
	regs.x.cx = count * 4;
	regs.x.dx = index;
	/* Write palette data to the transfer buffer */
	dosmemput(palette[ index ], count * 4, __tb);
	/* Load the transfer buffer */
	regs.x.es = far2seg(__tb);
	regs.x.di = far2off(__tb);
	return vbeFunction(VBE_DAC_DATA, &regs);
}

/*******************************************************************************
 *
 *	VBE_PROTECTED_MODE_INTERFACE (0ah)
 *
 *	This function returns a pointer to a table that contains code for a 32
 *	bit protected mode interface that can be either copied into local 32 bit
 *	memory space or can be executed from ROM providing the calling
 *	application sets all required selectors and I/O access correctly.
 *
 ******************************************************************************/

/*
 * vbeGetProtectedModeInterface
 *
 *	VBEprotectedModeInterface * pInterface
 *		Pointer to a data structure in which protected mode information
 *		will be stored.
 *	returns
 *		Buffer for which the members of 'vbeProtectedModeInterface' are
 *		offsets to. NULL if the function failed.
 */
char *vbeGetProtectedModeInterface(VBEprotectedModeInterface * pInterface)
{
	__dpmi_regs regs;
	regs.h.bl = 0x00; /* must be zero */
	if(vbeFunction(VBE_PROTECTED_MODE_INTERFACE, &regs))
	{
		/* Allocate a buffer to copy protected mode interface data */
		char *buffer = malloc(regs.x.cx);
		if(buffer)
		{
			/* Convert the segment:offset address to absolute */
			long address = near3far(regs.x.es, regs.x.di);
			/* Read the data address into our buffer */
			dosmemget(address, regs.x.cx, buffer);
			/* Copy the initial buffer into the offsets structure */
			memcpy(pInterface, buffer, sizeof(VBEprotectedModeInterface));
			/* Return the buffer so additional data can be parsed */
			return buffer;
		}
	}
	return NULL;
}

/*******************************************************************************
 *
 *	VBE_PIXEL_CLOCK (0bh)
 *
 *	Allows an application to determine if a particular pixel clock is
 *	available. When this function is called with bl set to 0, it will run
 *	the requested pixel clock through the internal PLL programming routines
 *	and return the actual pixel clock that will be programmed into the
 *	hardware. For hardware that is not fully programmable the returned pixel
 *	clock that is closest the one desired may be substantially different.
 *
 ******************************************************************************/

/*
 * vbeSetPixelClock
 *
 *	int frequency
 *		Requested pixel clock in units of Hz.
 *	returns
 *		Closest pixel clock available.
 */
int vbeSetPixelClock(int mode, int frequency)
{
	__dpmi_regs regs;
	regs.h.bl  = VBE_SET;
	regs.d.ecx = frequency;
	regs.x.dx  = mode;
	if(vbeFunction(VBE_PIXEL_CLOCK, &regs))
		return regs.d.ecx;
	else
		return -1;
}

/*
 * vbeGetPixelClock
 *
 *	returns
 *		Current pixel clock in units of Hz.
 */
int vbeGetPixelClock()
{
	__dpmi_regs regs;
	regs.h.bl = VBE_GET;
	if(vbeFunction(VBE_PIXEL_CLOCK, &regs))
		return regs.d.ecx;
	else
		return -1;
}

/*******************************************************************************
 *
 *	VBE_POWER_MANAGEMENT (10h)
 *
 *	Not yet supported.
 *	Supplemental
 *	Signature "VBE/PM"
 *
 *	Defines a function that application software can use to control the
 *	power saving features of VESA DPMS compliant displays or a flat panel
 *	controller. They will allow an application to set display power states
 *	without specific hardware knowledge or direct access. The hardware
 *	mechanism for controlling the power states of display devices is defined
 *	by the VESA Display Power Management Signaling (DPMS) standard.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	VBE_FLAT_PANEL_INTERFACE (11h)
 *
 *	Not yet supported.
 *	Supplemental
 *
 *	This proposed optional supplemental specification allows access to the
 *	special features incorporated in flat panel controllers. There is no
 *	reference specification at the time of this standards approval.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	VBE_AUDIO_INTERFACE (13h)
 *
 *	Not yet supported.
 *	Supplemental
 *	Signature "VBE/AI"
 *
 *	This optional function provides standard audio services.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	VBE_OEM (14h)
 *
 *	Not yet supported.
 *	Supplemental
 *
 *	This optional supplemental function provides OEM's with a code dispatch
 *	area that falls under the VESA functions. An OEM may use this area at
 *	their own risk. VESA states no warranties or guarantees about the
 *	function calls contained within this area.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 *	VBE_DISPLAY_DATA_CHANNEL (15h)
 *
 *	Not yet supported.
 *	Supplemental
 *	Signature "VBE/DCC"
 *
 *	The DDC standard defines a set of functions to retrieve the Extended
 *	Display Identification Data (EDID) structure from the display over the
 *	Display Data Channel. The hardware mechanism and identity information
 *	content can be retrieved from the display devices is defined by the VESA
 *	Display Data Channel (DDC) standard.
 *
 ******************************************************************************/
