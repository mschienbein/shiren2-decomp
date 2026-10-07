#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x4]; void *actor_4; void *target_8; u8 padC[0x10]; void *unit_1C; } Obj80112864;
typedef struct { s32 words[6]; } Buf80112864;
void func_801124F8(void *src, void *actor, void *unit, Buf80112864 *out);
void func_800A7ADC(void *target, void *payload);
void func_80112864(void *ctx, Obj80112864 *obj) {
    Buf80112864 buf;
    func_801124F8(ctx, obj->actor_4, obj->unit_1C, &buf);
    func_800A7ADC(obj->target_8, &buf);
}
