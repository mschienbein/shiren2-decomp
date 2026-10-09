#include "common.h"

typedef struct Ent Ent;
extern s32 func_800AE9AC(Ent *, s32, s32);

s32 func_8010B840(Ent *object) {
    return func_800AE9AC(object, 2, 0) == 0;
}
