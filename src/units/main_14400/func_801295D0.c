#include "common.h"

typedef unsigned char u8;

u8 *func_801295D0(u8 *obj, u8 *cursor)
{
    obj[0xB9] = *cursor;
    return cursor + 1;
}
