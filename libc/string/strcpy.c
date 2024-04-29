#include <string.h>

char *strcpy(char *restrict dst, const char *restrict src)
{
    if(!dst || !src)
        return NULL;
    
    char *temp = dst;
    while(*dst++ = *src++);
    return temp;
}