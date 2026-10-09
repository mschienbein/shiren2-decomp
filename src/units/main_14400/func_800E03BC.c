#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { Position position; u8 field_08[0x2B]; u8 field_33; } Object;
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Object *object);
extern void func_800497F0(s32 id, ...);
extern void func_800E039C(Object *object, s32 value);
static inline void copy_position(Position *out, Position *in) {
    out->x = in->x;
    out->y = in->y;
}
void func_800E03BC(Object *object, s32 value) {
    u8 level = value;
    Position position;
    s32 event;
    copy_position(&position, &object->position);
    event = func_80049CB4(0xDA, &position);
    if (object->field_33 < level) {
        func_800497F0(0x192, event, func_800A3B20(object));
    } else if (level < object->field_33) {
        func_800497F0(0x190, event, func_800A3B20(object));
    }
    func_800E039C(object, level);
    func_80049CB4(0x1F, object);
}
