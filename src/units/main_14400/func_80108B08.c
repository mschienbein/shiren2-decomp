#include "common.h"

typedef unsigned char u8;

typedef struct Obj80108B08 {
    u8 pad_00[0xA0];
    u8 count_A0;
} Obj80108B08;

void func_80108B08(Obj80108B08 *obj) {
    obj->count_A0--;
}
