#include "send_gprmc.h"

int main ( int argc, char * argv[] ) {
    char buff[1024];
    format(buff, sizeof(buff), 2016, 6, 5, 10, 59, 00);
    
    puts(buff);
    
    auto a = 1.24f;
    
    printf("%d\n", a);
    
    return 0;
}