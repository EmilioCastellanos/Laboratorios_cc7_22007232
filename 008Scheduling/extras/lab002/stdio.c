#include "stdio.h"
#include "os.h"
#include <stdarg.h>

#define BUF_SIZE 64

void PRINT(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[BUF_SIZE];
    int i = 0;

    while (*fmt)
    {
        if (*fmt == '%' && *(fmt + 1))
        {
            fmt++;

            if (*fmt == 's')
            {
                char *s = va_arg(args, char *);
                OS_WRITE(s);
            }
            else if (*fmt == 'd')
            {
                int d = va_arg(args, int);
                OS_ITOA(d, buffer);
                OS_WRITE(buffer);
            }
            else
            {
                OS_WRITE("%");

                char tmp[2] = {*fmt, '\0'};
                OS_WRITE(tmp);
            }
        }
        else
        {
            char tmp[2] = {*fmt, '\0'};
            OS_WRITE(tmp);
        }
        fmt++;
    }

    va_end(args);
}

void READ(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[BUF_SIZE];

    if (*fmt == '%' && *(fmt + 1) == 'd')
    {
        int *out = va_arg(args, int *);
        OS_READ(buffer, BUF_SIZE);
        *out = OS_ATOI(buffer);
    }

    va_end(args);
}
