#include "common.h"

typedef unsigned char u8;

/* Views shared with func_8009D910 (src descriptor and dims record). */
typedef struct { u8 b[4]; } Dims8009D910;
typedef struct { s32 w[4]; } Desc8009D910;
typedef struct { u8 pad0[0x24]; s32 field_24; u8 pad28[0x20]; s32 field_48; } Obj8009D910;

extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8009D910(Obj8009D910 *obj, Desc8009D910 *src, Dims8009D910 *dims);
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];

void func_80096BD0(Obj8009D910 *obj, Desc8009D910 *src) {
    Dims8009D910 arg;
    Dims8009D910 dims;
    s32 active;

    func_8006A810(dims.b, 0, 4);
    dims.b[0] = (u8)src->w[0];
    dims.b[1] = 1;
    dims.b[3] = (u8)src->w[0];
    arg = dims;
    func_8009D910(obj, src, &arg);
    active = D_80140160[4] == 1;
    if (active) {
        obj->field_48 = 0x3D0900;
    }
}
