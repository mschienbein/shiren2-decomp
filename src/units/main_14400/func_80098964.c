#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    s32 x;
    s32 y;
} Pos80098964;

typedef struct {
    u8 pad0[0x20];
    s32 unit_20;
} Obj80098964;

extern s32 func_80098E34(Obj80098964 *obj, s32 pos);
extern void *func_800980F0(void *c, s32 idx);

/* Widget +0x8C returns an opaque selection token. This override uses the item
 * pointer itself; the default override encodes its numeric selection value. */
void *func_80098964(Obj80098964 *self, Pos80098964 pos) {
    return func_800980F0(self, func_80098E34(self, pos.x + self->unit_20 * pos.y));
}
