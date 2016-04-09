/*******************************************************************************
 *
 *	Routines for working with segmented memory and far pointers.
 *	Specific to DJGPP
 *
 */

#include <pc.h>
#include <dos.h>
#include <crt0.h>
#include <go32.h>
#include <dpmi.h>
#include <sys/farptr.h>
#include <sys/nearptr.h>
#include <sys/movedata.h>

#ifndef farseg_h
#define farseg_h

long near2far(long address);

long near3far(long segment, long offset);

short far2seg(long address);

short far2off(long address);

long far2near(long address);

void farmemsetb(long segment, long index, long length, char value);

void nearmemcpyb(long nearptr, char *buffer, char delimiter);

void nearmemcpyw(long nearptr, short *buffer, short delimiter);

#endif /* farseg_h */
