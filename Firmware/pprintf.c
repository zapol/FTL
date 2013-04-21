
//#include <stdio.h>
#include <stdarg.h>
#include "pprintf.h"


int _pp_vprintf(void (*cb) (int), char const *format, va_list ap)
{
	unsigned char scratch[16];
	unsigned char format_flag;
	unsigned int u_val = 0;
	unsigned char base;
	unsigned char *ptr;
	unsigned char width = 0;
	unsigned char fill;

	while (1)
	{

		width = 0;
		fill = ' ';
		while ((format_flag = *format++) != '%')
		{
			if (!format_flag)
			{
				va_end(ap);
				return (0);
			}

			cb(format_flag);
		}


		// check for zero pad
		format_flag = *format - '0';
		if (format_flag == 0)	// zero pad
		{
			fill = '0';
			format++;
		}

		// check for width spec
		format_flag = *format - '0';
		if (format_flag > 0 && format_flag <= 9)	// width set
		{
			width = format_flag;
			format++;
		}

		switch (format_flag = *format++)
		{
			case 'c':
				format_flag = va_arg(ap, int);

				//fall through

			default:
				cb(format_flag);

				continue;

			case 'S':
			case 's':
				ptr = (unsigned char *) va_arg(ap, char *);
				while (*ptr)
					cb(*ptr++);
				continue;



			case 'd':
			case 'u':
				base = 10;
				goto CONVERSION_LOOP;

			case 'x':
				base = 16;

			  CONVERSION_LOOP:

				u_val = va_arg(ap, unsigned int);

				if (u_val & 0x80000000 && base == 10)
				{
					u_val = -u_val;
					cb('-');
				}
				ptr = scratch + 16;

				*--ptr = 0;

				do
				{
					char ch = (u_val % base) + '0';
					if (ch > '9')
						ch += 'A' - '9' - 1;

					*--ptr = ch;

					u_val /= base;

					if (width)
						width--;

				}
				while (u_val > 0);

				while (width--)
					*--ptr = fill;

				while (*ptr)
					cb(*ptr++);

		}
	}
	return 0;
}

int _s_vprintf(char *buf, char const *format, va_list ap)
{
	unsigned char scratch[16];
	unsigned char format_flag;
	unsigned int u_val = 0;
	unsigned char base;
	unsigned char *ptr;
	unsigned char width = 0;
	unsigned char fill;
	unsigned int index=0;

	while (1)
	{

		width = 0;
		fill = ' ';
		while ((format_flag = *format++) != '%')
		{
			if (!format_flag)
			{
				va_end(ap);
				buf[index++]=0;
				return (0);
			}

			buf[index++]=format_flag;
		}


		// check for zero pad
		format_flag = *format - '0';
		if (format_flag == 0)	// zero pad
		{
			fill = '0';
			format++;
		}

		// check for width spec
		format_flag = *format - '0';
		if (format_flag > 0 && format_flag <= 9)	// width set
		{
			width = format_flag;
			format++;
		}

		switch (format_flag = *format++)
		{
			case 'c':
				format_flag = va_arg(ap, int);

				//fall through

			default:
				buf[index++]=format_flag;

				continue;

			case 'S':
			case 's':
				ptr = (unsigned char *) va_arg(ap, char *);
				while (*ptr)
					buf[index++]=(*ptr++);
				continue;



			case 'd':
			case 'u':
				base = 10;
				goto CONVERSION_LOOP;

			case 'x':
				base = 16;

			  CONVERSION_LOOP:

				u_val = va_arg(ap, unsigned int);

				if (u_val & 0x80000000 && base == 10)
				{
					u_val = -u_val;
					buf[index++]='-';
				}
				ptr = scratch + 16;

				*--ptr = 0;

				do
				{
					char ch = (u_val % base) + '0';
					if (ch > '9')
						ch += 'A' - '9' - 1;

					*--ptr = ch;

					u_val /= base;

					if (width)
						width--;

				}
				while (u_val > 0);

				while (width--)
					*--ptr = fill;

				while (*ptr)
					buf[index++]=(*ptr++);

		}
	}
	buf[index++]=0;
	return 0;
}

int pprintf(void (*cb) (int), char const *format, ...)
{
	va_list ap;
	va_start(ap, format);
	return _pp_vprintf(cb, format, ap);
}

int mysprintf(char *buf, char const *format, ...)
{
	va_list ap;
	va_start(ap, format);
	return _s_vprintf(buf, format, ap);
}
