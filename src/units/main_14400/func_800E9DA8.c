#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef struct List800CD3D0 List800CD3D0;
typedef struct { u8 pad0[0x98]; s16 adjust98; s16 pad9A; u8 *(*inventory9C)(u8 *); u8 padA0[8]; s16 adjustA8; s16 padAA; void (*updateAC)(void *); } Methods;
typedef struct { u8 pad0[0x24]; const Methods *methods24; } Actor;
typedef struct { s32 index0; void *container4; s32 active8; void *currentC; } Iter;
typedef Iter S;
typedef struct { u8 pad0[2]; u8 flags2; u8 pad3[2]; s8 field5; } Ent;
extern u32 D_8013960C;
extern S *func_800CEB20(S *s, void *a);
extern s32 func_800CEBA0(Iter *);
extern Ent *func_800CEC68(Iter *);
extern void func_800CD3D0(List800CD3D0 *list, u32 index);
/* Inventory/actor slot +0xB4 supplies kind; this base override ignores it. */
void func_800E9DA8(Actor *actor, s32 kind) {
    List800CD3D0 *list;
    Iter iterator;
    D_8013960C <<= 1;
    actor->methods24->updateAC((u8 *)actor + actor->methods24->adjustA8);
    D_8013960C >>= 1;
    list = (List800CD3D0 *)actor->methods24->inventory9C((u8 *)actor + actor->methods24->adjust98);
    if (list) {
        func_800CEB20(&iterator, list);
        while (func_800CEBA0(&iterator)) {
            Ent *item = func_800CEC68(&iterator);
            s32 remove = 0;
            if (~item->field5 || (item->flags2 >> 7)) remove = 1;
            if (remove) func_800CD3D0(list, iterator.index0 + 1);
        }
    }
}
