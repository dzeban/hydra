#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

static int number2str(unsigned int n, char *str)
{
	while (n)
	{
		*str++ = (n % 10) + '0';
		n /= 10;
	}
	return 0;
}

static int number2hex(unsigned int n, char *str)
{
    int c;

	while (n) {
        c = (n % 16);

        if (c >= 10) {
            c += 'A' - 10;
        } else {
            c += '0';
        }

		*str++ = c;
		n /= 16;
	}
	return 0;
}

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

		/* FIXME: should clear before printing next number into */
		/* String to convert number types.
		 * 20 chars is enough to store up to 64 bit types both in hex and dec.  */
		char number[20];

		if ( *(++format) == '%' )
			goto print_c;

		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if ( *format == 'd' )
		{
			format++;
			int d = (int) va_arg(parameters, int);
			if (number2str(d, number))
				return -1;
			print(number, strlen(number));
		}
		else if ( *format == 'u' )
		{
			format++;
			unsigned int u = (unsigned int) va_arg(parameters, unsigned int);
			if (number2str(u, number))
				return -1;
			print(number, strlen(number));
		}
        else if (*format == 'x') 
        {
            format++;
            unsigned int u = (unsigned int)va_arg(parameters, unsigned int);
            if (number2hex(u, number))
                return -1;
            print(number, strlen(number));
        }
		else
		{
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}
