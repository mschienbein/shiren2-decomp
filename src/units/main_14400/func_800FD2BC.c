#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 pad_0[2]; u8 flags; } Item;
typedef struct { u8 pad_0[0x90]; s16 offset; u16 pad_92; s32 (*action)(void *, s32, s32, u8, s32); } Table;
typedef struct { Pos position; u8 pad_8[0x14]; u16 flags; u8 pad_1E[6]; Table *table; u8 pad_28[0x78]; Item *item; u8 delay; } Obj;
extern s32 func_800E20CC(void *obj);
extern s32 func_800B4F74(Pos *pos);
extern Item *func_800AACE0(s32 kind);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800AD7E0(Item *item, void *pos, s32 notify);
extern void func_800AE974(Item *item, s8 value);
extern u32 D_8013960C;
/* Monster action slot +0xB4 supplies a target pointer; this override ignores it. */
s32 func_800FD2BC(Obj *obj, void *target)
{
    Pos position;
    if ((func_800E20CC(obj) ^ 1) != 0) {
        return 0;
    }
    {
        Pos *point = &position;
        point->x = obj->position.x;
        point->y = obj->position.y;
        if (func_800B4F74(point) != 0) {
            return 0;
        }
    }
    if (obj->item == 0) {
        obj->item = func_800AACE0(0);
        if (obj->item == 0) {
            return 0;
        }
    }
    D_8013960C *= 2;
    obj->table->action((u8 *)obj + obj->table->offset, 0, 12, 255, 0);
    obj->table->action((u8 *)obj + obj->table->offset, 0, 1, 254, 0);
    obj->flags |= 1;
    if (func_800E20CC(obj) != 0) {
        func_80049CB4(222);
    }
    func_80049CB4(135, obj);
    D_8013960C >>= 1;
    func_80049CB4(288, &position);
    func_800AD7E0(obj->item, &position, 1);
    obj->item->flags |= 32;
    func_800AE974(obj->item, 2);
    obj->item = 0;
    obj->delay = 10;
    return 1;
}
