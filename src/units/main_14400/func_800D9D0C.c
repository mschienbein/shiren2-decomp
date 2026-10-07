#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 data[0x18];
} Entry800D9D0C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    Entry800D9D0C *entry;
} Obj800D9D0C;

extern Entry800D9D0C D_80143330[2];

s32 func_800D9D0C(Obj800D9D0C *obj, u8 *out) {
    s32 i;
    Entry800D9D0C *p;

    *out++ = obj->unk1;
    for (i = 0, p = D_80143330; i < 2; i++, p++) {
        if (obj->entry == p) {
            break;
        }
    }
    *out = i;
    return 2;
}
