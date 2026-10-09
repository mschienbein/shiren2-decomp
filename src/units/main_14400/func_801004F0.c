#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct Actor Actor;
typedef struct { u8 pad_00[0xB0]; s16 delta; s16 pad_B2; s32 (*call)(void *, Actor *); } VTable;
struct Actor { u8 pad_00[0x24]; VTable *field_24; u8 pad_28[0x30]; Actor *field_58; };
typedef Actor Obj800F0EC4;
extern s32 func_800F0EC4(Obj800F0EC4 *obj);
extern s32 func_800F3358(Actor *actor);

s32 func_801004F0(Actor *actor) {
    Actor *target = actor->field_58;
    if (target != 0 && (func_800F0EC4(actor) ^ 1) != 0) {
        if (actor->field_24->call((u8 *)actor + actor->field_24->delta, target)) {
            return 1;
        }
    }
    return func_800F3358(actor);
}
