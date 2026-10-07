#include "common.h"
typedef struct { s32 field_0; s32 field_4; s32 field_8; unsigned char pad[0x420 - 0xC]; } Entry80053AE8;
extern Entry80053AE8 D_80161724[];
void func_80081D84(s32 handle);
void func_80053AE8(s32 index) {
    Entry80053AE8 *e = &D_80161724[index];
    if (e->field_0 != -1) {
        func_80081D84(e->field_0);
        e->field_0 = -1;
        e->field_8 = 0;
    }
}
