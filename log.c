#include <stdio.h>

#ifdef DEBUG
#define PRINT_D(x, ...) fprintf( stdout, "%s:%s:%d: " x "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
#define  PRINT_D(x, ...)
#endif

void main()
{
    PRINT_D("HELLO \n");
}