#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

/* Slot +0x6C binds u32 func_800E0E88/func_800E96D4; the result is narrowed into the s16 field. */
typedef struct { u8 pad0[0x68]; s16 offset_68; s16 pad6A; u32 (*fn_6C)(void *self); } VTable801237B8;
typedef struct { u8 pad0[0x1E]; u8 flags_1E; u8 pad1F[5]; VTable801237B8 *vtable_24; } Obj801237B8;
typedef struct { void *field_0; s32 field_4; s32 field_8; s16 value_C; s16 padE; s32 field_10; s32 field_14; } Info801237B8;
s32 func_800A58B8(void *obj);
u32 func_800B1C6C(void *pos);
void func_80123894(void *owner, void *obj, void *context);
void func_80136910(void *info, void *obj, u32 a, u32 b, u32 c);
void func_800A7ADC(void *target, void *payload);

static inline s32 check801237B8(void *ctx) {
    s32 ok = 0;

    if (func_800A58B8(ctx) == 1) {
        u16 flags = func_800B1C6C(ctx) & 0x2000;
        ok = flags != 0;
    }
    return ok;
}

void func_801237B8(void *owner, Obj801237B8 *obj, void *ctx, s32 force) {
    Info801237B8 info;

    if (force || check801237B8(ctx)) {
        func_80136910(&info, obj, 1, 6, 0);
        if (obj != 0 && (obj->flags_1E & 0x7C)) {
            info.value_C = (s16)obj->vtable_24->fn_6C((u8 *)obj + obj->vtable_24->offset_68);
        }
        func_800A7ADC(ctx, &info);
    } else {
        func_80123894(owner, obj, ctx);
    }
}
