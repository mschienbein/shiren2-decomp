#include "common.h"
/* B4F74 consumes both signed coordinate words, at +0 and +4. */
typedef struct Pos800BCB18 { s32 x; s32 y; } Pos800BCB18;
typedef struct Obj {
    unsigned char pad_00[8];
    Pos800BCB18 pos_08;
} Obj;
extern s32 func_800B4F74(Pos800BCB18 *pos);
s32 func_800CFBE8(Obj *self) {
    return func_800B4F74(&self->pos_08) != 0;
}
