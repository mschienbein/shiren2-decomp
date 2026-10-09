#include "common.h"

typedef struct { s32 unknown00[2]; void *field08; } Object;
extern char D_80153AA0[];
extern void func_800AC68C(Object *);
/* D_801605B0+0x0C destructor slot: flags is s32 like the peer destructors' slot type. */
void func_8012742C(Object *object, s32 flags) {
    object->field08 = D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
