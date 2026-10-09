#include "common.h"
typedef unsigned char u8;
/* The receiver is supplied by the D_8015C788+0xA4 method slot and is unused by the loader. */
extern u8 *func_80044D74(void *receiver, u8 index);
u8 *func_80109F30(void *receiver, u8 index) {
    return func_80044D74(receiver, index);
}
