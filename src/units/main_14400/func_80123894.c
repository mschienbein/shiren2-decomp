#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x10]; u16 field_10; } Owner80123894;
typedef struct { u8 pad0[0xA]; u8 kind_A; u8 padB[0x13]; u8 flags_1E; } Obj80123894;
void func_800A06F4(void *ctx, u16 id, Obj80123894 *obj, s32 mode, s32 flag);

void func_80123894(Owner80123894 *owner, Obj80123894 *obj, void *ctx) {
    s32 mode = 0x17;
    s32 flag;

    if (obj != 0 && !(obj->flags_1E & 0xC)) {
        mode = 6;
    }
    flag = 0;
    if (obj != 0) {
        flag = obj->kind_A != 0x37;
    }
    func_800A06F4(ctx, owner->field_10, obj, mode, flag);
}
