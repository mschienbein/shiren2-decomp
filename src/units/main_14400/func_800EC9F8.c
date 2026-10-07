#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x95]; u8 bits[32]; } Obj800EC9F8;
s32 func_800EC9F8(Obj800EC9F8 *obj, u8 index) {
    return (obj->bits[index >> 3] >> (index % 8)) & 1;
}
