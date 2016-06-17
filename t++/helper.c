#include "helper.h"

#ifdef WIN32
#   include <io.h>
#   define access _access
#   define F_OK 0
#else
#   include <unistd.h>
#endif

int file_exits(const char * file_name) {
    return access(file_name, F_OK ) != -1;
}
