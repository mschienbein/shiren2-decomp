#include "common.h"

typedef struct {
    unsigned char pad0[0xA4];
    s32 field_A4;
} Obj80108AEC;

s32 func_80108AEC(Obj80108AEC *obj) {
    return obj->field_A4;
}
