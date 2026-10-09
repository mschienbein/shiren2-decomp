#include "common.h"
typedef struct { unsigned char pad_00[8]; s32 channel_08; } Pak;
typedef struct { Pak *pak; unsigned char pad_04[8]; s32 ready_0C; } Context;
typedef struct { unsigned char pad_00[12]; Context *field_0C; } Object;
extern unsigned char D_801E028C[];
extern s32 func_8002E930(void *queue, Pak *pak, s32 channel);
s32 func_80131328(Object *object) {
    Context *context = object->field_0C;
    Pak *pak = context->pak;
    s32 result;
    context->ready_0C = 0;
    result = func_8002E930(D_801E028C, pak, pak->channel_08);
    if (result == 0) context->ready_0C = 1;
    return result;
}
