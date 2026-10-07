#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 b[4]; } Dims8009D910;
typedef struct { s32 w[4]; } Desc8009D910;
typedef struct { u8 pad0[0x24]; s32 field_24; u8 pad28[0x28]; Dims8009D910 dims; } Obj8009D910;
void func_8009543C(Obj8009D910 *obj, Desc8009D910 *desc);

void func_8009D910(Obj8009D910 *obj, Desc8009D910 *src, Dims8009D910 *dims) {
    Desc8009D910 desc;

    obj->dims = *dims;
    desc = *src;
    desc.w[0] = dims->b[0];
    func_8009543C(obj, &desc);
    obj->field_24 = (obj->dims.b[3] - 1) / dims->b[0] + 1;
}
