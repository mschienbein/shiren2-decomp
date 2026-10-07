#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x28]; s16 this_offset; u8 pad2A[2]; void (*method)(void *self, s32 kind, void *arg); } VTable80128A78;
typedef struct { u8 pad0[0x18]; VTable80128A78 *vtable; } Target80128A78;
/* The stream restores the complete four-byte group at +0xC, not a scalar byte. */
typedef struct { u8 field_C; u8 field_D; u8 field_E; u8 padF; } Restored80128A78;
typedef struct { u8 pad0[0xC]; Restored80128A78 fields_C; u8 field_10; } Obj80128A78;
typedef struct { u8 pad0[0x13]; u8 field_13; } Entry80128A78;
extern u8 D_80160714[];
void func_800AF174(Obj80128A78 *obj, Target80128A78 *target);
void func_800CA4E8(Target80128A78 *target, void *desc);
Entry80128A78 *func_80044FDC(u8 a, u8 b);
void func_80128A78(Obj80128A78 *obj, Target80128A78 *target) {
    VTable80128A78 *vt;
    func_800AF174(obj, target);
    func_800CA4E8(target, D_80160714);
    vt = target->vtable;
    vt->method((u8 *)target + vt->this_offset, 4, &obj->fields_C);
    obj->field_10 = func_80044FDC(obj->fields_C.field_D, obj->fields_C.field_E)->field_13;
}
