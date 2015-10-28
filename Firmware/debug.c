#include "debug.h"
#include "pprintf.h"
#include "maint_usart.h"

extern void dbgPutch(int c);

int debug(char const *format, ...)
{
	va_list ap;
	va_start(ap, format);
#ifdef DEBUG
	return _pp_vprintf(maintPutch, format, ap);
#else
	return 0;
#endif
}
