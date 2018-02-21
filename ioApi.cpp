#include "ioApi.h"
#include <stdio.h>
#include <stdarg.h>

namespace io {

static bool _g_debug = false;
bool is_debug() { return _g_debug; }
void set_debug(bool b) { _g_debug = b; }

void _debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

}
