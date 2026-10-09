#include "common.h"

typedef unsigned char u8;
/* The receiver is supplied by the D_8015C690+0xA4 method slot and is unused by the loader. */
extern void *func_80044DCC(void *receiver, u8 index);

void *func_8010984C(void *receiver, u8 index)
{
    return func_80044DCC(receiver, index);
}
