/*******************************************************************************
 *
 *	VESA VBE Framework Tools
 *
 *	Based on the official specification
 *	http://www.vesa.org/public/VBE/vbe3.pdf
 *	Portions of code from DJ Delorie
 *	http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
 *	http://www.delorie.com/djgpp/doc/ug/graphics/vbe20.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "farseg.h"
#include "VESA.h"

/*
 * vesaEnumerateModes
 *
 *	Iterates across all the video modes supported by the SVGA card calling a
 *	user supplied function on each to test for video mode attributes.
 *
 *	VESAcontext *context
 *		Pointer to a VBE context data structure. This structure is used
 *		to store data on available modes. When the function returns this
 *		structure will be set to the mode which returned true under the
 *		'test'. In the event that no mode was accepted this structure is
 *		cleared.
 *	BOOL test(VBEmodeInfo *info)
 *		A function pointer which is called on every video mode in the
 *		enumeration. When this function returns true the enumeration
 *		stops and the video mode tested is returned.
 *	returns
 *		A video mode number for which 'test' returned true, or -1 if no
 *		mode passed 'test'.
 */

int vesaEnumerateModes(VESAcontext *pContext, BOOL(*test)(VBEmodeInfo *pModeInfo))
{
	int mode, index = 0;
	while(TRUE)
	{
		mode = pContext->SupportedModes[index++];
		/* Value -1 marks end of the list */
		if(mode == -1)
		{
			memset(&pContext->ModeInfo, 0, sizeof(VBEmodeInfo));
			break;
		}
		/* Read mode information */
		if(vbeGetModeInfo(&pContext->ModeInfo, mode))
		{
			/* Run test on this mode */
			if(test(&pContext->ModeInfo))
			{
				break;
			}
		}
	}
	return mode;
}

/*
 * vesaPhysicalAddressMapping
 *
 *	Attempts physical address mapping of the linear frame buffer for the
 *	supplied video mode so that pixel data can be written it in a linear
 *	fashion.
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 *	returns
 *		A linear address which points to the beginning of the linear
 *		frame buffer on the SVGA card, or -1 if the address was unable
 *		to be mapped.
 */

BOOL vesaPhysicalAddressMapping(VESAcontext *pContext)
{
	/* Point to video memory */
	pContext->VideoMapping.address = pContext->ModeInfo.PhysicalBasePointer;
	/* Calculate size of memory */
	pContext->VideoMapping.size = pContext->BIOSInfo.TotalMemory << 16;
	/* Map address in linear memory */
	return __dpmi_physical_address_mapping(&pContext->VideoMapping) == 0;
}

/*
 * vesaUnmapPhysicalAddress
 *
 *	Releases the physical address mapping created in the previous function.
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 */

void vesaUnmapPhysicalAddress(VESAcontext *pContext)
{
	__dpmi_free_physical_address_mapping(&pContext->VideoMapping);
}

/*
 * vesaLock
 *
 *	Enables near pointer access to the linear frame buffer in video memory.
 *	While in "locked" mode all memory protection is disabled.
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 *	returns
 *		An absolute memory addressing pointer to the linear frame buffer
 *		of the SVGA card, or NULL if near pointers were not enabled.
 */

void *vesaLock(VESAcontext *pContext)
{
	/* Check if near pointers are available */
	if(_crt0_startup_flags & _CRT0_FLAG_NEARPTR)
	{
		/* Attempt enabling absolute memory addressing */
		if(__djgpp_nearptr_enable())
		{
			/* Recalculate address as offset to DJGPP base */
			return (void *)(pContext->VideoMapping.address + __djgpp_conventional_base);
		}
	}
	return NULL;
}

/*
 * vesaUnlock
 *
 *	Disables near pointer access to the linear frame buffer.
 */

void vesaUnlock()
{
	/* Disable absolute memory addressing */
	__djgpp_nearptr_disable();
}

/*
 * vesaPhysicalAddressSelector
 *
 *	Allocates an LDT descriptor which can be used as the segment selector in
 *	far pointer memory addressing with _farpoke function calls.
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 *	returns
 *		A data segment pointer which can be used to address the linear
 *		frame buffer using poke and peek calls.
 */

BOOL vesaPhysicalAddressSelector(VESAcontext *pContext)
{
	/* Map address in linear memory */
	if(vesaPhysicalAddressMapping(pContext) != -1)
	{
		/* Allocate an LDT descriptor to access the region */
		long selector = __dpmi_allocate_ldt_descriptors(1);
		if(selector != -1)
		{
			/* Set the descriptor location and size */
			__dpmi_set_segment_base_address(selector, pContext->VideoMapping.address);
			__dpmi_set_segment_limit(selector, pContext->VideoMapping.size - 1);
			/* Return the LDT descriptor */
			pContext->VideoSelector = selector;
			return TRUE;
		}
		/* Release the video memory map */
		vesaUnmapPhysicalAddress(pContext);
	}
	return FALSE;
}

/*
 * vesaFreeAddressSelector
 *
 *	Free's an LDT descriptor created in the previous function.
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 *	long selector
 *		The segement selector to release.
 */

void vesaFreeAddressSelector(VESAcontext *pContext)
{
	/* Release the LDT descriptor */
	__dpmi_free_ldt_descriptor(pContext->VideoSelector);
	/* Release the video memory map */
	vesaUnmapPhysicalAddress(pContext);
}

/*
 * vesaCreateProtectedModeInterface
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 *	returns
 *		An allocated memory buffer containing the protected mode code
 *		block, or NULL if the interface failed to be created.
 */

BOOL vesaCreateProtectedModeInterface(VESAcontext *pContext)
{
	VBEprotectedModeInterface pmi;
	char *buffer = vbeGetProtectedModeInterface(&pmi);
	if(buffer)
	{
		pContext->PMIBuffer    = buffer;
		pContext->BankSwitch   = buffer + pmi.BankSwitchOffset;
		pContext->DisplayStart = buffer + pmi.DisplayStartOffset;
		pContext->DACData      = buffer + pmi.DACDataOffset;
		return TRUE;
	}
	return NULL;
}

/*
 * vesaDestroyProtectedModeInterface
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 */

void vesaDestroyProtectedModeInterface(VESAcontext *pContext)
{
	free(pContext->PMIBuffer);
	pContext->PMIBuffer    = NULL;
	pContext->BankSwitch   = NULL;
	pContext->DisplayStart = NULL;
	pContext->DACData      = NULL;
}

/*
 * vesaSetBankPosition
 *
 *	Calls the bank switching function in the code segment allocated with
 *	'vesaCreateProtectedModeInterface'.
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 *	int window
 *		Indicates which window will be moved.
 *			VBE_WINDOW_A
 *			VBE_WINDOW_B
 *	int number
 *		The new position for the window bank.
 */

void vesaSetBankPosition(VESAcontext *pContext, int window, int number)
{
	asm
	(
	    " call *%0 "
	    :
	    : "r"(pContext->BankSwitch),
	    "b"(window),
	    "d"(number)
	    : "%eax",
	    "%ebx",
	    "%ecx",
	    "%edx",
	    "%esi",
	    "%edi"
	);
}

/*
 * vesaCreateVideoBIOSImage
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 */

BOOL vesaCreateVideoBIOSImage(VESAcontext *pContext)
{
	static long imageSegment = 0xc0000;
	static int  imageSize    = 32768;
	static int  scanSize     = imageSize - sizeof(VBEprotectedModeInfo);
	int iterator;
	/* Usually 32k? Is there a better size? */
	char *buffer = malloc(imageSize);
	/* Make a local copy of the BIOS image */
	dosmemget(imageSegment, imageSize, buffer);
	/* Scan the BIOS data image for the PMID signature */
	for(iterator = 0; iterator < scanSize; iterator ++)
	{
		if(strncmp(buffer + iterator, VBE_PMID_SIGNATURE, 4) == 0)
		{
			int count;
			int sum = 0;
			/* Do a checksum evaluation on the bytes inside the PM header */
			for(count = 0; count < sizeof(VBEprotectedModeInfo); count ++)
			{
				sum += buffer[ iterator + count ];
			}
			if(sum == 0)
			{
			}
		}
	}
	free(buffer);
	return FALSE;
}

/*
 * vesaDestroyVideoBIOSImage
 *
 *	VESAcontext * pContext
 *		Pointer to a VBE context structure.
 */

void vesaDestroyVideoBIOSImage(VESAcontext *pContext)
{
}
