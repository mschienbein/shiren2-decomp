#include "common.h"
typedef struct { s32 x, y; } Position;
typedef struct { Position position; unsigned char pad8[0x2A]; unsigned char level32; unsigned char pad33[0x42]; unsigned char kind75; unsigned char pad76[4]; unsigned char max7A; unsigned char pad7B[0x1F]; unsigned short flags9A; unsigned char pad9C; unsigned char level9D; } Object;
extern s32 func_800E0F40(void *object);
extern char *func_800A3B20(void *object);
extern void func_800F04EC(void *object, short delta);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern s32 func_800E4454(void *object);
extern void func_800F05EC(void *object);
extern s32 func_800A08D8(s32 mode, s32 key, s32 selection);
void func_800F23A4(Object *object, short delta) {
    Position old_position;
    s32 old_kind = func_800E0F40(object);
    s32 old_level = object->level32;
    char *old_name = func_800A3B20(object);
    if (object->flags9A & 0x40) {
        s32 before = object->level9D;
        func_800F04EC(object, delta);
        if (before < object->level9D) {
            func_80049CB4(0x80, object);
            func_800498E4(0x29, old_name);
        } else if (object->level9D < before) func_800498E4(0x12A, old_name);
    } else {
        s32 maximum = object->max7A;
        s32 level = old_level + delta;
        if (maximum < level) level = maximum;
        else if (level <= 0) level = 1;
        object->level32 = level;
        if ((unsigned char)old_kind != (unsigned char)func_800E0F40(object)) {
            s32 message;
            Position *position;
            s32 status = func_800E4454(object);
            status ^= 1;
            if (status) object->kind75 = func_800E0F40(object);
            old_position.x = object->position.x;
            position = &old_position;
            position->y = object->position.y;
            func_80049CB4(0x132);
            func_80049CB4(6);
            func_800F05EC(object);
            func_80049CB4(7);
            func_80049CB4(6);
            if (delta > 0) { func_80049CB4(0x80, object); message = 0x27; }
            else { func_80049CB4(0x109, position); message = 0x28; }
            func_80049CB4(7);
            func_80049CB4(6);
            func_80049CB4(0x20, object, delta);
            func_80049CB4(7);
            func_800498E4(message, old_name, func_800A3B20(object));
            func_800A08D8(1, func_80049CB4(0xDA, &old_position), 0);
        }
    }
}
