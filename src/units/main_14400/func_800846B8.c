#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef short s16;

typedef struct {
    u8 pad0[0x4];
    s16 field_4;
} Obj;

extern s32 D_8013E924;
/* Reads no argument (0x8005484C..0x80054858); no original caller supplies one. */
void func_8005484C(void);

void func_800846B8(Obj *obj) {
    func_8005484C();
    D_8013E924 = 1;
    obj->field_4 = 4;
}
