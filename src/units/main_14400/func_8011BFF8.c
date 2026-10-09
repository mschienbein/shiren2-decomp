#include "common.h"

typedef struct { s32 x; s32 y; } Pos800AC6F8;
typedef struct { Pos800AC6F8 min; Pos800AC6F8 max; } Rect800AC6F8;
extern Rect800AC6F8 D_801429C0;
extern void func_800AC6F8(Rect800AC6F8 *rect, s32 allowSpecial);

void func_8011BFF8(void)
{
    func_800AC6F8(&D_801429C0, 1);
}
