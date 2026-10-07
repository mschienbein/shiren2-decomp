#include "common.h"

typedef struct { char pad0[0x1C]; char *unk1C; } Obj;
extern Obj *D_8013E81C;
void func_800837F4(void) {
    s32 i = 0;
    Obj *obj = D_8013E81C;
    s32 j;
    for (; i < 14; i++) {
        for (j = 0; j < 6; j++) {
            *(short *)(obj->unk1C + (i * 12 + j * 2)) = 0x2FF;
        }
    }
}
