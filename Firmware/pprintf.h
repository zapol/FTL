#ifndef PPRINTF_H
#define PPRINTF_H

#include <stdarg.h>

int pprintf(void (*cb) (int), char const *format, ...);
int _pp_vprintf(void (*cb) (int), char const *format, va_list ap);
int _s_vprintf(char *buf, char const *format, va_list ap);
int mysprintf(char *buf, char const *format, ...);

#endif							//PPRINTF+H
