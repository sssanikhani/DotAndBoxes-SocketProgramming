#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "consts.h"
#include "io_lib.h"


void please_scan(char *format, ...) {

    va_list ap;
    va_start(ap, format);

    char buff[BUFF_SIZE];
    read(STDIN_FILENO, buff, sizeof(buff));
    vsscanf(buff, format, ap);

    va_end(ap);

}


void please_print(char *format, ...) {
    va_list ap;
    
    va_start(ap, format);

    char str[BUFF_SIZE];
    vsprintf(str, format, ap);
    write(STDOUT_FILENO, str, strlen(str));

    va_end(ap);

}



void set_color(int color) {

    switch (color)
    {
    case RESET:
        please_print("\033[0m");
        break;

    case BLUE:
        please_print("\033[1;34m");
        break;

    case RED:
        please_print("\033[1;31m");
        break;

    case GREEN:
        please_print("\033[1;32m");
        break;

    case YELLOW:
        please_print("\033[01;33m");
        break;
    
    default:
        please_print("\033[0m");
        break;
    }

}
