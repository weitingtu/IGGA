#ifndef IO_H
#define IO_H

#define io_debug(format, ...) if (io::is_debug()) { _debug(format, ##__VA_ARGS__); }

namespace io {

bool is_debug();
void set_debug(bool b);
void _debug(const char* format, ...) __attribute__((format(printf, 1, 2)));

}


#endif // IO_H
