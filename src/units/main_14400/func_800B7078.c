#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;


typedef struct {
    u8 pad0[0x18];
    s16 this_offset;
    u8 pad1A[2];
    void (*release)(void *self);
} VTable800B7078;
typedef struct { u8 pad0[0x24]; VTable800B7078 *vtable; } Obj800B7078;
typedef struct { s32 x; s32 y; } Pos800B7078;

s32 func_80046240(void);
void func_800AA4BC(void);
s32 func_800AA4D0(void);
Obj800B7078 *func_800AA4E4(void);
Obj800B7078 *func_800AA5F8(void);
s32 func_800A5B98(Obj800B7078 *obj, Pos800B7078 *pos);
void func_800A58FC(Obj800B7078 *obj, Pos800B7078 *pos);
void func_800EE718(Obj800B7078 *obj, s32 mode);
/* Base override of slot +0x2C of the D_80153B40 family: void (self). */
void func_800B7078(void *self /* unused: the slot call contract supplies the receiver */) {
    Pos800B7078 pos;
    Obj800B7078 *obj;
    s32 blocked = 0;
    if (func_80046240() == 0) {
        u32 bit2 = ((D_80142F18.flags >> 2) & 1);
        if (bit2 != 0) {
            blocked = 1;
        } else {
            u32 bit4 = ((D_80142F18.flags >> 4) & 1);
            if (bit4 != 0) {
                blocked = 1;
            }
        }
    }
    if (blocked) {
        return;
    }
    func_800AA4BC();
    while (func_800AA4D0()) {
        obj = func_800AA4E4();
        if (obj == 0) {
            continue;
        }
        if (func_800A5B98(obj, &pos)) {
            func_800A58FC(obj, &pos);
            func_800EE718(obj, 2);
        } else {
            obj->vtable->release((u8 *)obj + obj->vtable->this_offset);
        }
    }
    obj = func_800AA5F8();
    if (obj != 0) {
        if (func_800A5B98(obj, &pos)) {
            func_800A58FC(obj, &pos);
        } else {
            obj->vtable->release((u8 *)obj + obj->vtable->this_offset);
        }
    }
}
