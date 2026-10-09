#include "common.h"
typedef unsigned char u8;
/* Initialized mode byte; the original image starts with value 1. */
u8 D_80148D60 = 1;
void func_8012EA50(u8 mode)
{
    D_80148D60 = mode;
}
