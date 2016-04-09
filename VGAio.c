
/*******************************************************************************
 *
 *	VGA I/O library
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

#include "farseg.h"
#include "VGAio.h"

/*******************************************************************************
 *
 *	Color/Monochrome Registers
 *
 ******************************************************************************/

BOOL vgaIsColorMode(void)
{
	return inportb(VGA_MISC_OUTPUT_READ) & VGA_IO_ADDRESS_SELECT_BIT;
}

void vgaResolveCRTCAddresses(void)
{
	if(vgaIsColorMode())
	{
		VGA_CRTC_ADDRESS          = VGA_COLOR_CRTC_ADDRESS;
		VGA_CRTC_DATA             = VGA_COLOR_CRTC_DATA;
		VGA_INPUT_STATUS_1        = VGA_COLOR_INPUT_STATUS_1;
		VGA_FEATURE_CONTROL_WRITE = VGA_COLOR_FEATURE_CONTROL_WRITE;
	}
	else
	{
		VGA_CRTC_ADDRESS          = VGA_MONO_CRTC_ADDRESS;
		VGA_CRTC_DATA             = VGA_MONO_CRTC_DATA;
		VGA_INPUT_STATUS_1        = VGA_MONO_INPUT_STATUS_1;
		VGA_FEATURE_CONTROL_WRITE = VGA_MONO_FEATURE_CONTROL_WRITE;
	}
}

/*******************************************************************************
 *
 *	Vertical Retrace Synchronous
 *
 ******************************************************************************/

void vgaOnSync(void)
{
	/* Wait until done with vertical retrace */
	while(inportb(VGA_INPUT_STATUS_1) & VGA_VERTICAL_RETRACE_BIT);
	/* Wait until done refreshing */
	while(!(inportb(VGA_INPUT_STATUS_1) & VGA_VERTICAL_RETRACE_BIT));
}

/*******************************************************************************
 *
 *	Display Attribute Controller
 *
 ******************************************************************************/

void vgaWritePalette(int index, char red, char green, char blue)
{
	outportb(VGA_DAC_ADDRESS_WRITE_MODE, index);
	outportb(VGA_DAC_DATA, red);
	outportb(VGA_DAC_DATA, green);
	outportb(VGA_DAC_DATA, blue);
}

void vgaWritePaletteRange(char palette[256][3 , int index, int count)
{
	int iterator;
	outportb(VGA_DAC_ADDRESS_WRITE_MODE, index);
	for(iterator = 0; iterator < count; iterator ++)
	{
		outportb(VGA_DAC_DATA, palette[ index + iterator ][ 0 ]);
		outportb(VGA_DAC_DATA, palette[ index + iterator ][ 1 ]);
		outportb(VGA_DAC_DATA, palette[ index + iterator ][ 2 ]);
	}
}

void vgaWriteEntirePalette(char palette[256][3])
{
	vgaWritePaletteRange(palette, 0, 256);
}

void vgaReadPalette(int index, char *red, char *green, char *blue)
{
	outportb(VGA_DAC_ADDRESS_READ_MODE, index);
	setsafe(red,   inportb(VGA_DAC_DATA));
	setsafe(green, inportb(VGA_DAC_DATA));
	setsafe(blue,  inportb(VGA_DAC_DATA));
}

void vgaReadPaletteRange(char palette[256][3], int index, int count)
{
	int iterator;
	outportb(VGA_DAC_ADDRESS_READ_MODE, index);
	for(iterator = 0; iterator < count; iterator ++)
	{
		palette[ index + iterator ][ 0 ] = inportb(VGA_DAC_DATA);
		palette[ index + iterator ][ 1 ] = inportb(VGA_DAC_DATA);
		palette[ index + iterator ][ 2 ] = inportb(VGA_DAC_DATA);
	}
}

void vgaReadEntirePalette(char palette[256][3])
{
	vgaReadPaletteRange(palette, 0, 256);
}

/*******************************************************************************
 *
 *	Query
 *
 ******************************************************************************/

int vgaQuery(int port, int index, int data)
{
	outportb(port, index);
	return inportb(data);
}

int vgaQueryCRTC(int index)
{
	return vgaQuery(VGA_CRTC_ADDRESS, index, VGA_CRTC_DATA);
}

int vgaQueryAttribute(int index)
{
	return vgaQuery(VGA_ATTRIBUTE_ADDRESS, index, VGA_ATTRIBUTE_DATA_READ)
}

int vgaQuerySequencer(int index)
{
	return vgaQuery(VGA_SEQUENCER_ADDRESS, index, VGA_SEQUENCER_DATA);
}

int vgaQueryGraphics(int index)
{
	return vgaQuery(VGA_GRAPHICS_ADDRESS, index, VGA_GRAPHICS_DATA);
}

/*******************************************************************************
 *
 *	Query Graphics
 *
 ******************************************************************************/

int vgaQueryGraphicsMode(int mask)
{
	return vgaQueryGraphics(VGA_GRAPHICS_MODE) & mask;
}

int vgaQueryMiscGraphics(int mask)
{
	return vgaQueryGraphics(VGA_MISC_GRAPHICS) & mask;
}

/*******************************************************************************
 *
 *	Query Sequencer
 *
 ******************************************************************************/

BOOL vgaQuerySequencerSetting(int bit)
{
	return vgaQuerySequencer(VGA_RESET) & bit;
}
