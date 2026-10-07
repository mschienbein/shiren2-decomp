#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u16 D_80157470[], D_801574E8[], D_80157180[], D_80157248[];
extern u16 D_801575E0[], D_80157728[], D_80157648[];
extern u8 D_801575A0[];
s32 func_8010C174(u8 *obj, u8 kind, u16 *a, u8 *b, u16 *c, u16 *d, u16 *e, u16 *f,
                  u16 *g, u16 *h);
s32 func_8010D668(u8 *obj) {
    return func_8010C174(obj, obj[1], D_80157470, D_801575A0, D_801574E8, D_80157180, D_80157248,
                        D_801575E0, D_80157728, D_80157648);
}
