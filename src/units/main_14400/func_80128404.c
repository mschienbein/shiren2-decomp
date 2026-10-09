#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Spawn descriptor: flag bits, kind/variant pair and a level delta. */
typedef struct {
    u8 pad00[0xC];
    u8 flags;
    u8 kind;
    u8 variant;
    u8 delta;
} Spawn80128404;

typedef struct {
    u8 pad00[0x55];
    u8 field_55;
    u8 pad56[0x72 - 0x56];
    u8 field_72;
    u8 pad73[0x9A - 0x73];
    u16 field_9A;
} Obj80128404;

/* Third argument is nullable placement memory; null asks the factory to allocate. */
extern void *func_800A8694(u8 kind, u8 variant, void *memory);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800F04EC(Obj80128404 *obj, s16 delta);
extern void func_800E4D88(Obj80128404 *obj, s32 value);

/* Create the object described by spawn and apply its flags. */
Obj80128404 *func_80128404(Spawn80128404 *spawn) {
    Obj80128404 *obj = func_800A8694(spawn->kind, spawn->variant, 0);

    if (obj == 0) {
        return 0;
    }
    obj->field_72 = (obj->field_72 | 4) & ~8;
    if (spawn->flags & 1) {
        obj->field_72 |= 8;
    }
    if (!(spawn->flags & 2)) {
        return obj;
    }
    obj->field_9A |= 0x40;
    if (spawn->flags & 4) {
        obj->field_9A |= 0x200;
    }
    func_80049CB4(0x1F, obj);
    func_800F04EC(obj, spawn->delta);
    switch (obj->field_55 & 0xF) {
    case 1:
        func_800E4D88(obj, 2);
        break;
    case 3:
        func_800E4D88(obj, 4);
        break;
    }
    return obj;
}
