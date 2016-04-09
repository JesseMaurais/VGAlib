/*******************************************************************************
 *
 *	VBE/AF
 *
 *	Accelerator Functions
 */

#ifndef VBEaf_h
#define VBEaf_h

/*
 * Tells compiler to allocate structure with data fields packed to bit or byte
 * alignment. This is needed since the vbe BIOS which will be queried for this
 * info will return it with this type of alignment. The code generated  by this
 * compiler won't have any control over the origin of these structures.
 */

#pragma pack(1)

/*******************************************************************************
 *
 *	VESA BIOS Extension Accelerator Function flags
 *
 ******************************************************************************/

#define VBEAF_DRV				"VBEAF.DRV"
#define VBEAF_PATH				"VBEAF_PATH"
#define VBEAF_VERSION				0x0100

#define AF_HAVE_MULTI_BUFFER			0x0001
#define AF_HAVE_VIRTUAL_SCROLL			0x0002
#define AF_HAVE_BANKED_BUFFER			0x0004
#define AF_HAVE_LINEAR_BUFFER			0x0008
#define AF_HAVE_2D_ACCELERATION			0x0010
#define AF_HAVE_DUAL_BUFFERS			0x0020
#define AF_HAVE_HARDWARE_CURSOR			0x0040
#define AF_HAVE_8_BIT_DAC			0x0080
#define AF_NON_VGA_MODE				0X0100

#define AF_VIRTUAL_SCROLL			0x1000	/* Enable virtual scrolling */
#define AF_MULTI_BUFFER				0x2000	/* Enable multiple buffers */
#define AF_LINEAR_FRAME_BUFFER			0x4000	/* Enable linear frame buffer */
#define AF_NO_CLEAR				0x8000	/* Don't clear display memory */

typedef enum
{
	AF_REPLACE,		/* Replace target with incoming pixel */
	AF_AND,			/* Bitwise AND the pixels together */
	AF_OR,			/* Bitwise OR target with incoming pixel */
	AF_XOR,			/* Exclusive OR target with incoming pixel */
	AF_NOP			/* Destination pixel unchanged */

} AFmixMode;

typedef enum
{
	AF_OK,			/* No error */
	AF_NOT_DETECTED,	/* Graphics hardware not detected */
	AF_DRIVER_NOT_FOUND,	/* Driver file not found */
	AF_CORRUPT_DRIVER,	/* File loaded not a driver */
	AF_OUT_OF_MEMORY,	/* Not enough memory to load driver */
	AF_OLD_VERSION,		/* Driver file is an older version */
	AF_MEMORY_MAP		/* Could not map physical memory areas */

} AFerror;

typedef struct AFdeviceContext
{
	/****** Device driver header block ************************************/

	char            Signature[12];		/* 'VBEAF.DRV\0' 12 byte signature */
	unsigned int    Version;		/* Driver interface version */
	unsigned int    DriverRevision;		/* Driver revision number */
	char            OEMVendorName[80];	/* Vendor name string */
	char            OEMCopywrite[80];	/* Vendor copywrite string */
	short          *AvailableModes;		/* Offset to supported modes table */
	unsigned int    TotalMemory;		/* Amount of memory in kb detected */
	unsigned int    Attributes;		/* Driver attributes */
	unsigned int    BankSize;		/* Bank size in kb (4 or 64) */
	unsigned int    BankedBasePointer;	/* Physical address of banked buffer */
	unsigned int    LinearSize;		/* Linear buffer size in kb */
	unsigned int    LinearBasePointer;	/* Physical address of linear buffer */
	unsigned int    LinearGranularity;	/* Linear bit granularity in bytes */
	unsigned short *IOPortsTable;		/* Offset of I/O ports table */
	unsigned int	IOMemoryBase[4];	/* Base address of I/O memory maps */
	unsigned int	IOMemoryLength[4];	/* Length of I/O memory maps */
	unsigned int	Reserved1[10];		/* Reserved for future expansion */

	/***** Near pointers mapped by application for driver *****************/

	void           *IOMemoryMaps[4];	/* Pointers to mapped I/O memory */
	void           *BankedMemory;		/* Pointer to mapped banked video memory */
	void           *LinearMemory;		/* Pointer to mapped linear video memory */

	/***** Important selectors allocated by application for driver ********/

	unsigned int    Selector0000;		/* 1mb selector to entire first mb */
	unsigned int    Selector0040;		/* Selector to segment at 0x0040:0 */
	unsigned int    SelectorA000;		/* Selector to segment at 0xA000:0 */
	unsigned int    SelectorB000;		/* Selector to segment at 0xB000:0 */
	unsigned int    SelectorC000;		/* Selector to segment at 0xC000:0 */

	/***** Device driver state variables **********************************/

	unsigned int    BufferEndX;		/* Last X coord of each buffer */
	unsigned int    BufferEndY;		/* Last Y coord of each buffer */
	unsigned int    OriginOffset;		/* Current start of active page */
	unsigned int    OffscreenOffset;	/* Start of offscreen memory area */
	unsigned int    OffscreenStartY;	/* First Y coord of offscreen mem */
	unsigned int    OffscreenEndY;		/* Last Y coord of offscreen mem */
	unsigned int    Reserved2[10];		/* Reserved for future expansion */

	/***** Relocateable 32 bit bank switch routine ************************/

	unsigned int    SetBank32Length;	/* Length of 32 bit code */
	void           *SetBank32;		/* 32 bit relocateable code */

	/***** Required callback functions provided by application ************/

	void           *Interrupt86;		/* Issue real mode interrupt */
	void           *CallRealMode;		/* Call a real mode function */

	/***** Device driver functions ****************************************/

	void           *InitDriver;		/* Initialise driver */
	void           *GetVideoModeInfo;	/* Get video mode information */
	void           *SetVideoMode;		/* Set a video mode */
	void           *RestoreTextMode;	/* Restore text mode operation */
	void           *SetBank;		/* Set framebuffer bank */
	void           *SetDisplayStart;	/* Set virtual display start */
	void           *SetActiveBuffer;	/* Set active output buffer */
	void           *SetVisibleBuffer;	/* Set Visible display buffer */
	void           *SetPaletteData;		/* Program palette data */
	void           *SetGammaCorrectData;	/* Program gamma correct'n data */
	void           *WaitTillIdle;		/* Wait till engine is idle */
	void           *EnableDirectAccess;	/* Enable direct memory access */
	void           *DisableDirectAccess;	/* Disable direct memory access */
	void           *SetCursor;		/* Download hardware cursor */
	void           *SetCursorPos		/* Set cursor position */
	void           *SetCursorColor;		/* Set cursor color */
	void           *ShowCursor;		/* Show/hide cursor */
	void           *SetMix;			/* Set ALU mix operations */
	void           *Set8x8MonoPattern;	/* Set 8x8 mono bitmap pattern */
	void           *Set8x8ColorPattern;	/* Set 8x8 color bitmap pattern */
	void           *SetLineStipple;		/* Set 16 bit line stipple */
	void           *SetClipRect;		/* Set clipping rectangle */
	void           *DrawScan;		/* Draw a solid scanline */
	void           *DrawPattScan;		/* Draw a patterned scanline */
	void           *DrawColorPattScan;	/* Draw color pattern scanline */
	void           *DrawScanList;		/* Draw list of solid scanlines */
	void           *DrawRect;		/* Draw a solid rectangle */
	void           *DrawPattRect;		/* Draw a patterned rectangle */
	void           *DrawColorPattRect;	/* Draw color pattern rectangle */
	void           *DrawLine;		/* Draw a solid line */
	void           *DrawStippleLine;	/* Draw a stippled line */
	void           *DrawTrap;		/* Draw a solid trapezoid */
	void           *DrawTri;		/* Draw a solid triangle */
	void           *DrawQuad;		/* Draw a solid quad */
	void           *PutMonoImage;		/* Display a monochrome bitmap */
	void           *BitBlt;			/* Blt screen to screen */
	void           *BitBltLinear;		/* Linear source BitBlt */
	void           *TransBlt;		/* Blt scr/scr w/ transparency */
	void           *TransBltLinear;		/* Linear source TransBlt */

} AFdc;

typedef struct AFmodeInfo
{
	unsigned short Attributes;         /* Mode attributes */
	unsigned short XResolution;        /* Horizontal resolution in pixels */
	unsigned short YResolution;        /* Vertical resolution in pixels */
	unsigned short BytesPerScanLine;   /* Bytes per horizontal scan line */
	unsigned short BitsPerPixel;       /* Bits per pixel */
	unsigned short MaxBuffers;         /* Maximum num. of display buffers */

	/***** RGB pixel format info ******************************************/

	unsigned char  RedMaskSize;        /* Size of direct color red mask */
	unsigned char  RedFieldPosition;   /* Bit posn of lsb of red mask */
	unsigned char  GreenMaskSize;      /* Size of direct color green mask */
	unsigned char  GreenFieldPosition; /* Bit posn of lsb of green mask */
	unsigned char  BlueMaskSize;       /* Size of direct color blue mask */
	unsigned char  BlueFieldPosition;  /* Bit posn of lsb of blue mask */
	unsigned char  RsvdMaskSize;       /* Size of direct color res mask */
	unsigned char  RsvdFieldPosition;  /* Bit posn of lsb of res mask */

	/***** Virtual buffer dimensions **************************************/

	unsigned short MaxBytesPerScanLine;/* Maximum bytes per scan line */
	unsigned short MaxScanLineWidth;   /* Maximum pixels per scan line */
	unsigned char  Reserved[118];      /* Pad to 128 byte block size */

} AFinfo;

/*******************************************************************************
 *
 *	VBE Accelerator Functions
 *
 ******************************************************************************/

AFdc *afLoadDriver(const char *path);

void afUnloadDriver(AFdc *dc);

int afStatus();

const char *afErrorString(int status);

int afGetVideoModeInfo(AFdc *dc, int mode, AFinfo *info);

int afSetVideoMode(AFdc *dc, int mode, int *bytes, int nBuffers);

int afSetVirtualVideoMode(AFdc *dc, int mode, int w, int h, int *bytes, int nBuffers);

void afRestoreTextMode(AFdc *dc);

void afSetDisplayStart(AFdc *dc, int x, int y, int wait);

void afSetActiveBuffer(AFdc *dc, int index);

void afSetVisibleBuffer(AFdc *dc, int index, int wait);

void afSetPalette(AFdc *dc, AFpalette *palette, int index, int count, int wait);

void afSetGammaCorrect(AFdc *dc, AFpalette *palette, int index, int count);

void afSetBank(AFdc *dc, int bank);

void afWait(AFdc *dc);

void afEnableDirectAccess(AFdc *dc);

void afDisableDirectAccess(AFdc *dc);

void afSetCursor(AFdc *dc, int x, int y);

void afSetCursorPosition(AFdc *dc, int x, int y);

void afSetCursorColor(AFdc *dc, int red, int green, int blue);

void afShowCursor(AFdc *dc, int visible);

void afSetMix(AFdc *dc, int fore, int back);

void afSet8x8MonoPattern(AFdc *dc, AFpattern *palette);

void afSetLineStipple(AFdc *dc, AFstipple stipple);

void afSetClipRect(AFdc *dc, int xmin, int ymin, int xmax, int ymax);


/* TODO add drawing functions */


#endif /* VBEaf_h */

