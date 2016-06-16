#include "helper.h"

#include <unistd.h>

int file_exits(const char * file_name) {
    return access(file_name, F_OK ) != -1;
}
