#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x20]; void *field_20; } Sub8012CEC8;
typedef struct { u8 pad0[0x34]; Sub8012CEC8 *sub_34; } Obj8012CEC8;
extern u8 D_80148A58[];
extern u8 D_80148890[];
extern u8 D_801488F8[];
extern u8 D_801489A8[];
extern u8 D_80148A30[];
extern u8 D_80148980[];
extern Obj8012CEC8 *D_801DFF54;
void func_8012CB48(void *entry);
s32 func_80030644(void *filter, s32 paramID, void *param);

s32 D_80148A80 = 6;
void *D_80148A84[] = {
    D_80148A58, D_80148890, D_801488F8, D_801489A8, D_80148A30, D_80148980, 0,
};

s32 func_8012CEC8(s32 index) {
    Sub8012CEC8 *sub;

    if (index >= D_80148A80) {
        return 1;
    }
    func_8012CB48(D_80148A84[index]);
    sub = D_801DFF54->sub_34;
    func_80030644(sub->field_20, 1, sub);
    return 0;
}
