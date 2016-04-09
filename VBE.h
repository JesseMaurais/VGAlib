
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

#ifndef VBE_h
#define VBE_h

#include "VGA.h"

/*
 * Tells compiler to allocate structure with data fields packed to bit or byte
 * alignment. This is needed since the vbe BIOS which will be queried for this
 * info will return it with this type of alignment. The code generated  by this
 * compiler won't have any control over the origin of these structures.
 */

#pragma pack(1)

/*******************************************************************************
 *
 *	VESA BIOS Extension flags
 *
 ******************************************************************************/

#define VBE_VESA_SIGNATURE			"VESA"
#define VBE_PMID_SIGNATURE			"PMID"

#define VBE_VERSION_1_0				0x0100
#define VBE_VERSION_1_1				0x0101
#define VBE_VERSION_1_2				0x0102
#define VBE_VERSION_2_0				0x0200
#define VBE_VERSION_3_0				0x0300

/* VBE functions */

#define VBE_VESA_FUNCTION			0x4F	/* VESA base flag */
#define VBE_GET_INFO				0x00	/* Get SVGA information */
#define VBE_GET_MODE_INFO			0x01	/* Get SVGA mode information */
#define VBE_SET_MODE				0x02	/* Set SVGA video mode */
#define VBE_GET_MODE				0x03	/* Get current video mode */
#define VBE_VIDEO_STATE				0x04	/* Save/Restore video state */
#define VBE_BANK_SWITCH				0x05	/* Move readable/writable window space */
#define VBE_SCAN_LINE_LENGTH			0x06	/* Get/Set logical scan line length */
#define VBE_DISPLAY_START			0x07	/* Get/Set display start */
#define	VBE_DAC_BITS				0x08	/* Palette bit size control */
#define VBE_DAC_DATA				0x09	/* Get/Set palette data */
#define VBE_PROTECTED_MODE_INTERFACE		0x0A	/* Code for 32 bit protected mode */
#define VBE_PIXEL_CLOCK				0x0B	/* Get/Set pixel clock in units of Hz */

/* Supplemental functions */

#define VBE_POWER_MANAGEMENT			0x10	/* Power state control of attached device */
#define VBE_FLAT_PANEL_INTERFACE		0x11	/* Special features for flat panel controllers */
#define VBE_AUDIO_INTERFACE			0x13	/* Provides standard audio services */
#define VBE_OEM					0x14	/* Provides OEM's with a code dispatch area */
#define VBE_DISPLAY_DATA_CHANNEL		0x15	/* Mechanism to extract data from attached devices */

/* Status flags */

#define VBE_SUPPORTED				0x4F	/* Function supported */
#define VBE_SUCCEEDED				0x00	/* Function call succeeded */
#define VBE_FAILED				0x01	/* Function call failed */
#define VBE_FAILED_WITH_CONFIGURATION		0x02	/* Invalid function for current configurations */
#define VBE_FAILED_WITH_SETTINGS		0x03	/* Invalid function for current video mode */

/* Video mode optional flags */

#define VBE_VESA_MODE				0x0100	/* Bit is set if this is a VESA defined video mode */
#define VBE_CRTC				0x0800	/* Set mode with specified CRTC refresh values */
#define VBE_AF					0x1800  /* Reserved for VBE/AF, must be clear */
#define VBE_LINEAR_FRAME_BUFFER			0x4000	/* Exclusively request linear frame buffer */
#define VBE_NO_CLEAR				0x8000	/* Don't zero video memory on display change */
#define VBE_MODE_MASK				0xFE00	/* Mask away everything but the mode number */

/* Standard VESA supported modes */

#define VBE_MODE_640x480_8			0x100
#define VBE_MODE_640x480_8			0x101
#define VBE_MODE_800x600_4			0x102
#define VBE_MODE_800x600_8			0x103
#define VBE_MODE_1024x768_4			0x104
#define VBE_MODE_1024x768_8			0x105
#define VBE_MODE_1280x1024_4			0x106
#define VBE_MODE_1280x1024_8			0x107
#define VBE_MODE_80x60				0x108
#define VBE_MODE_132x25				0x109
#define VBE_MODE_132x43				0x10A
#define VBE_MODE_132x50				0x10B
#define VBE_MODE_132x60				0x10C
#define VBE_MODE_320x200_5_5_5			0x10D
#define VBE_MODE_320x200_5_6_5			0x10E
#define VBE_MODE_320x200_8_8_8			0x10F
#define VBE_MODE_640x480_5_5_5			0x110
#define VBE_MODE_640x480_5_6_5			0x111
#define VBE_MODE_640x480_8_8_8			0x112
#define VBE_MODE_800x600_5_5_5			0x113
#define VBE_MODE_800x600_5_6_5			0x114
#define VBE_MODE_800x600_8_8_8			0x115
#define VBE_MODE_1024x768_5_5_5			0x116
#define VBE_MODE_1024x768_5_6_5			0x117
#define VBE_MODE_1024x768_8_8_8			0x118
#define VBE_MODE_1280x1024_5_5_5		0x119
#define VBE_MODE_1280x1024_5_6_5		0x11A
#define VBE_MODE_1280x1024_8_8_8		0x11B
#define VBE_MODE_SPECIAL_MEMORY_ACCESS		0x81FF

/* VBE_VIDEO_STATE */

#define VBE_STATE_HARDWARE			0x01	/* Save hardware state */
#define VBE_STATE_BIOS_DATA			0x02	/* Save BIOS state */
#define VBE_STATE_DAC				0x04	/* Save DAC state */
#define VBE_STATE_SVGA				0x08	/* Save SVGA card state */

#define VBE_QUERY				0x00	/* Return buffer size required to stave video state */
#define VBE_SAVE				0x01	/* Save video state into a local buffer */
#define VBE_RESTORE				0x02	/* Restore video state from a local buffer */

/* VBE_DISPLAY_START */

#define VBE_SET					0x00	/* Generic set used in several functions */
#define VBE_GET					0x01	/* Generic get used in several functions */
#define VBE_SET_SCHEDULE			0x02	/* Schedule a display start change */
#define VBE_SET_SCHEDULE_STEREO			0x03	/* Schedule a stereoscopic display start change */
#define VBE_QUERY_SCHEDULE			0x04	/* Get a scheduled display start */
#define VBE_ENABLE_STEREO_MODE			0x05	/* Enables stereoscopic mode */
#define VBE_DISABLE_STEREO_MODE			0x06	/* Disables stereoscopic mode */
#define VBE_SET_SYNC				0x80	/* Like VBE_SET but change is made on vertical retrace */
#define VBE_SET_SYNC_ALTERNATE			0x82	/* Same thing as VBE_SET_SYNC? */
#define VBE_SET_STEREO_SYNC			0x83	/* Like VBE_SET_SYNC but for stereoscopic mode */

/* VBEinfo.Capabilities */

#define VBE_SWITCHABLE_DAC			0x01

/* VBEmodeInfo.ModeAttributes */

#define VBE_MODE_HARDWARE			0x0001	/* Video mode is available on hardware */
#define VBE_MODE_TTY				0x0004	/* Supports TeleTYpe printing */
#define VBE_MODE_COLOR				0x0008	/* Mode is colored, or else monochrome */
#define VBE_MODE_GRAPHICS			0x0010	/* Mode is graphical, or else textual */
#define VBE_MODE_NON_VGA			0x0020	/* Not a standard VGA mode (XVGA?) */
#define VBE_MODE_NOT_BANKED			0x0040	/* Does not support bank switching */
#define VBE_MODE_LINEAR_FRAME_BUFFER		0x0080	/* Supports linear frame buffer */
#define VBE_MODE_DOUBLE_SCAN			0x0100	/* Supports double scan */
#define VBE_MODE_INTERLACE			0x0200	/* Video interlacing supported */
#define VBE_MODE_TRIPLE_BUFFER			0x0400	/* Hardware triple buffering supported */
#define VBE_MODE_STEREO				0x0800	/* Hardware stereoscopic display supported */
#define VBE_MODE_DUAL_DISPLAY_START_ADDRESS	0x1000	/* Multi-display addressing */

/* VBEmodeInfo.Win[A|B]Attributes */

#define VBE_WINDOW_SUPPORTED			0x01
#define VBE_WINDOW_READABLE			0x02
#define VBE_WINDOW_WRITABLE			0x04
#define VBE_WINDOW_A				0x00
#define VBE_WINDOW_B				0x01

/* VBEmodeInfo.MemoryModel */

#define	VBE_MODEL_TEXT				0x00	/* Not a graphics mode */
#define	VBE_MODEL_CGA				0x01	/* CGA compatibility */
#define	VBE_MODEL_HERCULES			0x02	/* Hercules adapter */
#define	VBE_MODEL_PLANAR			0x03	/* As in non-chain4 modes */
#define	VBE_MODEL_PACKED			0x04	/* Packed pixels */
#define	VBE_MODEL_XVGA				0x05	/* Non-chain4-VGA or ModeX */
#define	VBE_MODEL_RGB				0x06	/* Direct color */
#define	VBE_MODEL_YUV				0x07	/* Direct YUV */

/* VBEmodeInfo.DirectColorModeAttributes */

#define VBE_COLOR_RAMP_PROGRAMMABLE		0x01	/* Color ramps can be changed */
#define VBE_COLOR_ATTRIBUTE_USABLE		0x02	/* Usable by an application */

/* VBEcrtcInfo.Flags */

#define VBE_DOUBLE_SCANNED			0x01
#define VBE_INTERLACED				0x02
#define VBE_NEGATIVE_HORIZONTAL_SYNC_POLARITY	0x04
#define VBE_NEGATIVE_VERTICAL_SYNC_POLARITY	0x08

/*******************************************************************************
 *
 *	VESA BIOS Extension structures
 *
 ******************************************************************************/

/*
 * Basic VESA information block. Note that the string and mode list will be
 * physically located in the reserved section of this structure. This structure
 * holds basic information on the SVGA card capabilities as well as OEM data.
 */

typedef struct
{
	char           Signature[4];		/* 'VESA' 4 byte signature */
	unsigned short Version;			/* vbe version number */
	unsigned long  OEMStringPointer;	/* Pointer to OEM string */
	unsigned long  Capabilities;		/* Capabilities of video card */
	unsigned long  VideoModePointer;	/* Pointer to supported modes */
	unsigned short TotalMemory;		/* Number of 64kb memory blocks */
	unsigned short OEMSoftwareRevision;
	unsigned long  OEMVendorNamePointer;
	unsigned long  OEMProductNamePointer;
	unsigned long  OEMProductRevisionPointer;
	unsigned char  Reserved[222];
	unsigned char  OEMData[256];

} __attribute__((packed)) VBEinfo;

/*
 * Here follows the definition of the mode info block, obtained by passing the
 * mode number into interrupt 10h. This structure holds information specific
 * to a video mode.
 */

typedef struct
{
	unsigned short ModeAttributes;
	unsigned char  WindowAAttributes;
	unsigned char  WindowBAttributes;
	unsigned short WindowGranularity;	/* Granularity in kb */
	unsigned short WindowSize;		/* Size in kb */
	unsigned short WindowASegment;
	unsigned short WindowBSegment;
	unsigned long  WindowBankPointer;	/* Pointer to bank switching function */
	unsigned short BytesPerScanLine;
	/* VBE 1.2 */
	unsigned short XResolution;
	unsigned short YResolution;
	unsigned char  XCharSize;
	unsigned char  YCharSize;
	unsigned char  NumberOfPlanes;
	unsigned char  BitsPerPixel;
	unsigned char  NumberOfBanks;
	unsigned char  MemoryModel;
	unsigned char  BankSize;
	unsigned char  NumberOfImagePages;
	unsigned char  Reserved0;
	unsigned char  RedMaskSize;
	unsigned char  RedFieldPosition;
	unsigned char  GreenMaskSize;
	unsigned char  GreenFieldPosition;
	unsigned char  BlueMaskSize;
	unsigned char  BlueFieldPosition;
	unsigned char  ReservedMaskSize;
	unsigned char  ReservedFieldPosition;
	unsigned char  DirectColorModeAttributes;
	/* VBE 2.0 */
	unsigned long  PhysicalBasePtr;		/* Physical address for flat frame buffer  */
	unsigned long  ScreenMemoryOffset;	/* Pointer to start of off screen memory   */
	unsigned short ScreenMemorySize;	/* Amount of off screen memory in 1k units */
	/* VBE 3.0 */
	unsigned short LinearBytesPerScanLine;
	unsigned char  BankNumberOfPages;
	unsigned char  LinearNumberOfPages;
	unsigned char  LinearRedMaskSize;
	unsigned char  LinearRedFieldPosition;
	unsigned char  LinearGreenMaskSize;
	unsigned char  LinearGreenFieldPosition;
	unsigned char  LinearBlueMaskSize;
	unsigned char  LinearBlueFieldPosition;
	unsigned char  LinearReservedMaskSize;
	unsigned char  LinearReservedFieldPosition;
	unsigned long  MaxPixelClock;
	/* Pad to 256 bytes */
	unsigned char  Reserved1[190];

} __attribute__((packed)) VBEmodeInfo;

/*
 * This is the protected mode iterface block defined in vbe 2.0 specification.
 * It provides a code block for protected mode programs to execute common BIOS
 * functions on as well as providing a list of port addresses and memory
 * locations. It acts as the header to a larger block of code.
 */

typedef struct
{
	unsigned short BankSwitchOffset;
	unsigned short DisplayStartOffset;
	unsigned short DACDataOffset;
	unsigned short IOInfoOffset;

} __attribute__((packed)) VBEprotectedModeInterface;

/*
 * This is the CRTC timing structure defined in vbe 3.0 specification. It is an
 * optional second parameter to VBE_SET_MODE when VBE_CRTC flag is OR'ed with
 * the video mode number.
 */

typedef struct
{
	unsigned short HorizontalTotal;
	unsigned short HorizontalSyncStart;
	unsigned short HorizontalSyncEnd;
	unsigned short VerticalTotal;
	unsigned short VerticalSyncStart;
	unsigned short VerticalSyncEnd;
	unsigned char  Flags;
	unsigned long  PixelClock;
	unsigned short RefreshRate;
	unsigned char  Reserved0[40];

} __attribute__((packed)) VBEcrtcInfo;

/*
 * This is the protected mode information block defined in vbe 3.0 specification.
 * It is filled by scanning the code segment (c0000h) for the proper 'Signature'
 * and then double checking the data integrity with the Checksum field. This
 * feature is totaly optional and is not guaranteed to be present on all vbe 3.0
 * compliant hardware.
 */

typedef struct
{
	char           Signature[ 4 ];		/* 'PMID' 4 byte signature */
	unsigned short EntryPointOffset;	/* Protected mode entry point within BIOS */
	unsigned short InitializeOffset;	/* Protected mode initialization entry point */
	unsigned short BIOSDataSelector;	/* Selector to BIOS data area emulation block */
	unsigned short Selector_A0000;		/* Selector to access A0000h physical memory */
	unsigned short Selector_B0000;		/* Selector to access B0000h physical memory */
	unsigned short Selector_B8000;		/* Selector to access B8000h physical memory */
	unsigned short CodeSegmentSelector;	/* Selector to access code segment as data */
	unsigned char  InProtectedMode;		/* Set to 1 when in protected mode */
	unsigned char  Checksum;		/* Checksum byte for this structure */

} __attribute__((packed)) VBEprotectedModeInfo;

/*
 * Data structure to fill information on supplemental features.
 */

typedef struct
{
	unsigned char  Signature[7];		/* 'VBE/???' 7 byte signature */
	unsigned short Version;			/* Feature version number */
	unsigned char  Subfunctions;		/* Bitfield of supported subfunctions */
	unsigned short OEMSoftwareRevision;	/* OEM software revision number */
	unsigned long  OEMVendorNamePointer;	/* Far pointer to vendor name string */
	unsigned long  OEMProductNamePointer;	/* Far pointer to product name string */
	unsigned long  OEMRevisionPointer;	/* Far pointer to revision name string */
	unsigned long  OEMStringPointer;	/* Far pointer to OEM string */
	unsigned char  Reserved0[221];

} __attribute__((packed)) VBEsupplementalInfo;

/*******************************************************************************
 *
 *	VESA BIOS Extension functions
 *
 ******************************************************************************/

/* Error log */

const char *vbeGetErrorString();

/* VBE_GET_INFO */

BOOL vbeGetInfo(VBEinfo *info, short *modes, char *oem, char *vendor, char *name);

/* VBE_GET_MODE_INFO */

BOOL vbeGetModeInfo(VBEmodeInfo *info, int mode);

/* VBE_SET_MODE */

BOOL vbeSetMode(int mode, VBEcrtcInfo *info);

/* VBE_GET_MODE */

int vbeGetCurrentMode();

/* VBE_VIDEO_STATE */

int vbeQueryStateBufferSize(int flags);

BOOL vbeSaveState(int flags, void *buffer, int length);

BOOL vbeRestoreState(int flags, void *buffer, int length);

/* VBE_BANK_SWITCH */

BOOL vbeSetBankPosition(int window, int number);

int vbeGetBankPosition(int window);

/* VBE_SCAN_LINE_LENGTH */

typedef enum
{
	VBE_BYTES_PER_SCAN_LINE,
	VBE_PIXELS_PER_SCAN_LINE,
	VBE_NUMBER_OF_SCAN_LINES,
	VBE_NONE

} VBEreturns;

int vbeSetScanLineLength(int length, VBEreturns result);

int vbeGetScanLineLength(VBEreturns result);

/* VBE_DISPLAY_START */

BOOL vbeSetDisplayStart(int x, int y);

BOOL vbeGetDisplayStart(int *x, int *y);

BOOL vbeScheduleDisplayStartAddress(int address);

BOOL vbeScheduleStereoDisplayStartAddress(int leftAddress, int rightAddress);

BOOL vbeQueryScheduledStereoDisplayComplete();

BOOL vbeEnableStereoMode();

BOOL vbeDisableStereoMode();

BOOL vbeSetDisplayStartOnSync(int x, int y);

BOOL vbeSetDisplayStartAddressOnSync(int address);

BOOL vbeSetStereoDisplayStartOnSync(int x, int y);

/* VBE_DAC_BITS */

int vbeSetPaletteDepth(int bits);

int vbeGetPaletteDepth();

/* VBE_DAC_DATA */

BOOL vbeSetPalette(int index, int count, char palette[256][4]);

BOOL vbeGetPalette(int index, int count, char palette[256][4]);

BOOL vbeSetPaletteOnSync(int index, int count, char palette[256][4]);

/* VBE_PROTECTED_MODE_INTERFACE */

char *vbeGetProtectedModeInterface(VBEprotectedModeInterface *interface);

/* VBE_PIXEL_CLOCK */

int vbeSetPixelClock(int mode, int frequency);

int vbeGetPixelClock();


/* Tells compiler to use default C data alignment from now on */
#pragma pack()


#endif /* VBE_h */
