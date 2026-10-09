#include "common.h"
typedef struct { s32 x, y; } Position;
typedef struct { Position position; unsigned char pad8[0xDC]; unsigned short flagsE4; unsigned char padE6[0x22]; unsigned char field108; } Object;
extern const char D_80156959[];
extern void *func_800A38FC(s32 size);
extern s32 func_800E0F40(void *object);
extern void *func_800F83F0(void *object, unsigned char kind);
extern s32 func_800A3934(void *object);
extern s32 func_800A5DC4(void *object, Position *position, s32 mode);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(void *object);
extern void func_800498E4(s32 id, ...);
void func_800EBE14(Object *object) {
    Position origin, position;
    s32 any, i;
    origin.x = object->position.x;
    any = 0;
    origin.y = object->position.y;
    i = any;
    for (;;) {
        s32 remaining = i < 4;
        void *storage, *child;
        s32 placed;
        s32 status;
        if (!remaining) break;
        storage = func_800A38FC(0x80);
        child = func_800F83F0(storage, func_800E0F40(object));
        placed = 0;
        status = func_800A3934(child);
        status ^= 1;
        if (status) {
            position.x = object->position.x;
            position.y = object->position.y;
            placed = func_800A5DC4(child, &position, 3) != 0;
        }
        if (placed) { any = 1; func_80049CB4(0x78, child, &origin); }
        i++;
    }
    if (any) {
        unsigned short flags = object->flagsE4 | 0x10;
        object->field108 = D_80156959[0];
        object->flagsE4 = flags;
        func_800498E4(0x1A6, func_800A3B20(object));
    } else func_800498E4(0x223);
}
