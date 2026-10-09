#include "common.h"
typedef struct { s32 field_00, field_04; } Pair;
/* Operation object built by func_800C32C0 (vtable D_80153F70): +0x24 holds the item pointer
 * stored at 0x800C3374, +0xB8 the predicate word. func_800C37B8 passes its receiver here
 * (0x800C387C..0x800C3888). */
typedef struct {
    unsigned char field_00[0x24]; void *item_24;
    unsigned char field_28[0x90]; s32 field_b8;
} Object;
extern s32 func_800C3114(void *object, Pair *from, Pair *to, Pair *first, Pair *last);
extern s32 func_80049CB4(s32 command, ...);
void func_800C3754(Object *self, Pair *first, Pair *second) {
    Pair from, to;
    s32 command;
    if (func_800C3114(self, first, second, &from, &to)) {
        command = 0xb9;
        if (self->field_b8) command = 0xba;
        func_80049CB4(command | 0x1000, self->item_24, &from, &to);
    }
}
