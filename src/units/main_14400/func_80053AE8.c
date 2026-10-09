#include "common.h"
typedef struct { s32 field_0, field_4, field_8, field_C, field_10, field_14, field_18, field_1C; char messages_20[4][0x100]; } Slot;
extern Slot D_80161724[1];
void func_80081D84(s32 handle);
void func_80053AE8(s32 index) {
    Slot *e = &D_80161724[index];
    if (e->field_0 != -1) {
        func_80081D84(e->field_0);
        e->field_0 = -1;
        e->field_8 = 0;
    }
}
