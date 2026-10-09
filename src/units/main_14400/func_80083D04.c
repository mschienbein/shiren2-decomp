#include "common.h"

char *func_80083D04(char *destination, const char *source) {
    char *end = destination;
    while (*end != 0) {
        end++;
    }
    while ((*end++ = *source++) != 0) {
    }
    return destination;
}
