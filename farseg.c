/*******************************************************************************
 *
 *	Routines for working with segment:offset memory and near/far pointers.
 *
 */

#include "farset.h"

long near2far(long address)
{
	return ((address & 0xFFFF0000) >> 12) + (address & 0xFFFF);
}

long near3far(long segment, long offset)
{
	return ((segment & 0xFFFF) << 4) + (offset & 0xFFFF);
}

short far2seg(long address)
{
	return (address >> 4) & 0xFFFFF;
}

short far2off(long address)
{
	return address & 0xF;
}

long far2near(long address)
{
	return (far2seg(address) << 16) + far2off(address);
}

void farmemsetb(long segment, long buffer, long length, char value)
{
	int index;
	_farsetsel(segment);
	for(index = 0; index < length; index++)
	{
		_farnspokeb(buffer + index, value);
	}
}

char *nearmemcpyb(long nearptr, char *buffer, char delimiter)
{
	char value;
	long farptr = near2far(nearptr);
	int index = 0;
	_farsetsel(_dos_ds);
	do
	{
		value = _farnspeekb(farptr);
		buffer[ index ] = value;
		farptr ++;
		index ++;
	}
	while(value != delimiter);
	return buffer + index;
}

short *nearmemcpyw(long nearptr, short *buffer, short delimiter)
{
	short value;
	long farptr = near2far(nearptr);
	int index = 0;
	_farsetsel(_dos_ds);
	do
	{
		value = _farnspeekw(farptr);
		buffer[ index ] = value;
		farptr += 2;
		index ++;
	}
	while(value != delimiter);
	return buffer + index;
}
