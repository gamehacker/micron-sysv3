/*****************************************************************************
 * Micron System Version 3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <libc.h>
#include <device.h>
#include <config.h>

char *itoa(int value, char *str, int radix)
{
	int len=0, val = value;

	if(value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	/* evaluate target string length */
	while(val) {
		len++;
		val = val/radix;
	}

	/* cut down end of string */
	str[len] = '\0';

	/* do the conversion */
	while(len--) if((value%radix)>=0 && (value%radix)<=9) {
		str[len] = '0' + value%radix;
		value = value/radix;
	} else if((value%radix)>=10) {
		str[len] = 'a' + value%radix - 10;
		value = value/radix;
	}
	return str;
}

int strlen(char *str)
{
	int len=0;
	while(*str++!='\0') len++;
	return len;
}

int strcmp(char *str1, char *str2)
{
	int len1, len2;
	len1 = strlen(str1);
	len2 = strlen(str2);
	if(len1 != len2) {
		return -1;
	} else {
		while(len1--) if(*str1++ != *str2++) {
				return -1;
		}
	}
	return 0;
}

void *memcpy(void *dest, void *src, size_t cnt)
{
	char *bufd=dest, *bufs=src;
	while(cnt--) {
		*bufd++ = *bufs++;
	}
	return dest;
}

void *memset(void *dest, int ch, size_t cnt)
{
	char *buf = dest;
	while(cnt--) {
		buf[cnt] = ch;
	}
	return dest;
}

int __attribute__((format(printf, 1, 2)))
kprintf(char *fmt, ...)
{
	unsigned *args = (unsigned*)&fmt, argi=1;
	char buf[255];
	while(*fmt != '\0') switch(*fmt) {
	case '%':
		fmt++;
		switch(*fmt) {
		case 'b':
			itoa(args[argi++], buf, 2);
			DeviceWrite(CHRDEV, DEVID(CHR_TTY, 0), buf, 
					strlen(buf));
			break;
		case 'd':
			itoa(args[argi++], buf, 10);
			DeviceWrite(CHRDEV, DEVID(CHR_TTY, 0), buf, 
					strlen(buf));
			break;
		case 'x':
			itoa(args[argi++], buf, 16);
			DeviceWrite(CHRDEV, DEVID(CHR_TTY, 0), buf,
					strlen(buf));
			break;
		case 's':
			DeviceWrite(CHRDEV, DEVID(CHR_TTY, 0), 
				(char*)args[argi], strlen((char*)args[argi]));
			argi++;
			break;
		case 'c':
			DeviceWrite(CHRDEV, DEVID(CHR_TTY, 0),
					(char*)&args[argi++], 1);
			break;
		case 'C':
			DeviceIoctl(CHRDEV, DEVID(CHR_TTY, 0), 2, 
					(char)args[argi++]);
			break;
		case 'X':
			DeviceIoctl(CHRDEV, DEVID(CHR_TTY, 0), 4, 
					(char)args[argi++]);
			break;
		case 'Y':
			DeviceIoctl(CHRDEV, DEVID(CHR_TTY, 0), 5, 
					(char)args[argi++]);
			break;
		}
		fmt++;
		break;
	default:
		DeviceWrite(CHRDEV, DEVID(CHR_TTY, 0), fmt, 1);
		fmt++;
		break;
	}
	return 0;
}

