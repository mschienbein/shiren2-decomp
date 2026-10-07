#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x2]; s16 id; u8 pad4[0x3]; u8 mode; u8 pad8[0x36]; u8 frame; u8 pad3F; u8 field_40; } Ent800873C0;
typedef struct {
    u8 pad0[0x4];
    s16 state;
    u8 pad6[0x2];
    u16 step;
    u16 count;
    u8 padC[0x6];
    u16 flags;
    s32 entity;
    u8 pad18[0x4];
    s32 timer;
    u8 pad20[0x4];
    s32 baseFrame;
    s32 frame;
    u8 pad2C[0x30];
    s32 arg5C;
    s32 arg60;
    s32 arg64;
} Obj800873C0;
Ent800873C0 *func_8007946C(s32 arg0, s32 id);
s32 func_80077BA4(s32 kind, s32 value);
void func_8008B8A8(s32 entity, s32 arg1, s32 arg2, s32 arg3);
void func_800873C0(Obj800873C0 *obj) {
    Ent800873C0 *ent = func_8007946C(0, obj->entity);
    s32 frame;
    switch (obj->step) {
    case 0:
        frame = ent->frame;
        ent->mode = 2;
        obj->baseFrame = frame;
        obj->frame = frame % 8;
        obj->timer = 0x18;
        obj->step++;
        break;
    case 1:
        if (obj->timer-- != 0) {
            ent->field_40 = 0;
            if (--obj->frame < 0) {
                obj->frame = 7;
            }
            ent->frame = obj->frame;
            if (obj->flags & 0x1000) {
                func_80077BA4(ent->id, -1);
            }
            obj->count++;
        } else {
            ent->field_40 = 3;
            ent->frame = obj->baseFrame;
            ent->mode = 1;
            if (obj->flags & 0x1000) {
                func_80077BA4(ent->id, 0);
            }
            func_8008B8A8(obj->entity, obj->arg5C, obj->arg60, obj->arg64);
            obj->state = 4;
        }
        break;
    }
}
