/*******************************************************************************
 *
 *	VESA VBE Framework Tools
 *
 *	Based on the official specification
 *	http://www.vesa.org/public/VESA/vbe3.pdf
 *	Portions of code from DJ Delorie
 *	http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
 *	http://www.delorie.com/djgpp/doc/ug/graphics/vbe20.html
 */

#ifndef VESA_h
#define VESA_h

#include "VBE.h"

typedef struct
{
	/* VBE Data */
	VBEinfo BIOSInfo;
	VBEmodeInfo ModeInfo;
	/* Card Info */
	char OEMString[128];
	char VendorString[128];
	char ProductString[128];
	short SupportedModes[128];
	/* Video Memory Addressing */
	__dpmi_meminfo VideoMapping;
	unsigned long VideoSelector;
	/* Protected Mode */
	VBEprotectedModeInfo *BIOSHeader;
	char *BIOSImage;
	char *PMIBuffer;
	void *BankSwitch;
	void *DisplayStart;
	void *DACData;

} VESAcontext;

/* Video framework functions */

int vesaEnumerateModes(VESAcontext *context, BOOL(*)(VBEmodeInfo*));

/* Linear frame buffer mapping */

BOOL vesaPhysicalAddressMapping(VESAcontext *context);

void vesaUnmapPhysicalAddress(VESAcontext *context);

void *vesaLock(VESAcontext *context);

void vesaUnlock();

BOOL vesaPhysicalAddressSelector(VESAcontext *context);

void vesaFreeAddressSelector(VESAcontext *context);

/* Protected mode interface */

BOOL vesaCreateProtectedModeInterface(VESAcontext *context);

void vesaDestroyProtectedModeInterface(VESAcontext *context);

void vesaSetBankPosition(VESAcontext *context, int window, int number);

BOOL vesaCreateVideoBIOSImage(VESAcontext *context);

void vesaDestroyVideoBIOSImage(VESAcontext *context);


#endif /* VESA_h */
