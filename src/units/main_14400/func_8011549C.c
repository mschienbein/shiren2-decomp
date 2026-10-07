#include "common.h"

typedef unsigned char u8;

extern s32 func_800CD278(void *arg);

s32 func_8011549C(u8 *obj) {
    return func_800CD278(obj + 0xC);
}
