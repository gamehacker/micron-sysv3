/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, WL
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

void *memcpy( void *dest, const void *src, size_t count )
{

	while(count--)
	{
		dest[count] = src[count];
	}
	return dest;
}
