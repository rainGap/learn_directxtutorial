#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <windows.h>
#include "utils.h"


int getNum() {
	return 0;
}

void __cdecl odprintf(const char *format, ...)
{
	char buf[4096], *p = buf;
	va_list args;

	va_start(args, format);
	size_t s = sizeof(buf) - 1;
	int rc = _vsnprintf_s(buf, s, format, args);
	va_end(args);

	while (p > buf && isspace(p[-1]))
	{
		*--p = '\0';
		*p++ = '\r';
		*p++ = '\n'; *p = '\0';
	}
	OutputDebugString(buf);
}