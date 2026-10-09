#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_0[8]; s16 offset; u16 pad_A; void (*destroy)(void *, s32); } ItemTable;
typedef struct { u8 pad_0[8]; ItemTable *table; } Item;
typedef struct { u8 pad_0[0x18]; s16 offset; u16 pad_1A; void (*finish)(void *); } ObjectTable;
typedef struct { Pos position; u8 pad_8[2]; u8 kind; u8 pad_B[0x19]; ObjectTable *table; } Obj;
extern s32 func_800E0F40(Obj *obj);
extern char *func_80044FDC(u8 kind, u8 variant);
extern s32 func_800C587C(void *rng, u8 limit);
extern void func_800E2124(Obj *obj);
extern Item *func_800AAC48(s32 kind);
extern void func_800AE444(Item *obj, u8 value);
extern s32 func_800AD714(Item *obj, Pos *pos);
extern s32 func_800A60D8(Obj *obj, Pos *pos);
extern s32 func_800AE18C(Item *obj, Pos *pos);
extern s32 func_80049CB4(s32 id, ...);
extern void *func_800A65E4(Dir *out, Obj *obj, void *target);
extern void func_800A665C(Obj *obj, u8 *value);
extern u8 D_80147620[];
extern void *D_801476B8;
static __inline__ void destroy_item(Item *item)
{
    if (item != 0) {
        item->table->destroy((u8 *)item + item->table->offset, 3);
    }
}
void func_800F0130(Obj *obj)
{
    Pos position;
    Dir direction;
    u8 kind = obj->kind;
    if ((func_800C587C(D_80147620, ((u8 *)func_80044FDC(kind, func_800E0F40(obj) & 255))[6]) ^ 1) != 0) {
        func_800E2124(obj);
        if (kind == 41) {
            Item *item = func_800AAC48(0);
            if (item != 0) {
                s32 tries;
                func_800AE444(item, func_800E0F40(obj) & 255);
                position.x = obj->position.x;
                position.y = obj->position.y;
                tries = 100;
                for (;;) {
                    s32 retry = 0;
                    if (func_800AD714(item, &position) == 0) {
                        retry = tries-- > 0;
                    }
                    if (retry == 0) {
                        break;
                    }
                    func_800A60D8(obj, &position);
                }
                if (func_800AD714(item, &position) != 0) {
                    func_800AE18C(item, &position);
                    func_80049CB4(215, &position);
                } else {
                    destroy_item(item);
                }
            }
            obj->table->finish((u8 *)obj + obj->table->offset);
        }
    } else {
        func_800A65E4(&direction, obj, D_801476B8);
        func_800A665C(obj, &direction.value);
    }
}
