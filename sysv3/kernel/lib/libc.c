/*****************************************************************************
 * Micron System Version 3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <stdarg.h>
#include <libc.h>
#include <device.h>
#include <config.h>

char *itoa(int value, char *str, int radix)
{
	unsigned len=0, val = (unsigned)value, rad =(unsigned)radix;

	if(value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	/* evaluate target string length */
	while(val) {
		len++;
		val = val/rad;
	}

	/* cut down end of string */
	str[len] = '\0';

	/* do the conversion */
	while(len--) if((value%rad)>=0 && (value%rad)<=9) {
		str[len] = '0' + value%rad;
		value = value/rad;
	} else if((value%rad)>=10) {
		str[len] = 'a' + value%rad - 10;
		value = value/rad;
	}
	return str;
}

char *strchr(char *s, int c)
{
	while(*s != '\0') {
		if(*s == (char)c) {
			return s;
		}
		s++;
	}
	return 0;
}

char *strcpy(char *str1, char *str2)
{
	char *s1=str1, *s2=str2;
	while(*s2 != 0) {
		*s1++ = *s2++;
	}
	*s1 = 0;
	return str1;
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

int strncmp(char *str1, char *str2, size_t n)
{
	while(n--) {
		if(*str1++ != *str2++) {
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

int kprintf(const char *fmt, ...)
{
	char buf[256];
	unsigned *args = (unsigned*)&fmt, argi=1;
	while(*fmt != '\0') switch(*fmt) {
	case '%':
		fmt++;
		switch(*fmt) {
		case 'b':
			itoa(args[argi++], buf, 2);
			dev_write(CHRDEV, DEVID(CHR_TTY, 0), buf,
					strlen(buf));
			break;
		case 'd':
			itoa(args[argi++], buf, 10);
			dev_write(CHRDEV, DEVID(CHR_TTY, 0), buf,
					strlen(buf));
			break;
		case 'x':
			itoa(args[argi++], buf, 16);
			dev_write(CHRDEV, DEVID(CHR_TTY, 0), buf,
					strlen(buf));
			break;
		case 's':
			dev_write(CHRDEV, DEVID(CHR_TTY, 0),
				(char*)args[argi], strlen((char*)args[argi]));
			argi++;
			break;
		case 'c':
			dev_write(CHRDEV, DEVID(CHR_TTY, 0),
					(char*)&args[argi++], 1);
			break;
		case 'C':
			dev_ioctl(CHRDEV, DEVID(CHR_TTY, 0), 2,
					(char)args[argi++]);
			break;
		case 'X':
			dev_ioctl(CHRDEV, DEVID(CHR_TTY, 0), 4,
					(char)args[argi++]);
			break;
		case 'Y':
			dev_ioctl(CHRDEV, DEVID(CHR_TTY, 0), 5,
					(char)args[argi++]);
			break;
		}
		fmt++;
		break;
	default:
		dev_write(CHRDEV, DEVID(CHR_TTY, 0), (char*)fmt, 1);
		fmt++;
		break;
	}
	return 0;
}

//standard libc printf
int printf( const char *fmt, ...)
{
	va_list args;
	int i;
	char buf[256];
	va_start(args, fmt);
	i=vsprintf(buf, fmt, args);
    dev_write(CHRDEV, DEVID(CHR_TTY, 0),
            (char*)buf, i);
	va_end(args);
	return i;
}


int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;
	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}
