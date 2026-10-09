#include "common.h"
typedef unsigned char u8;
typedef struct { void *owner; s32 index; } Context;
typedef Context Entry800D01B8;
typedef struct MenuBase { char pad0[0x4C]; const void *vtbl; } MenuBase;
/* Five consecutive objects have this complete 0x40C-byte extent. */
typedef struct MenuPanel {
    MenuBase base;
    char pad50[0xC8 - 0x50];
    MenuBase second;
    char pad118[0x128 - 0x118];
    char unk128[0x194 - 0x128];
    const void *unk194;
    char pad198[0x2A0 - 0x198];
    const void *unk2A0;
    char pad2A4[0x2CC - 0x2A4];
    s32 field_2CC, field_2D0;
    char pad2D4[0x40C - 0x2D4];
} MenuPanel;
typedef MenuPanel Work;
typedef struct { u8 pad_0[0x64]; s32 selection_64[9]; } Object;
typedef struct Obj800EBA54 Obj800EBA54;
typedef struct Member800EBA54 Member800EBA54;
extern Obj800EBA54 *D_801476B8;
extern Work D_801404E0, D_801408EC, D_80141104;
extern u8 D_80138F98[];
extern Member800EBA54 *func_800EBA54(Obj800EBA54 *obj);
extern void func_80097B90(void *obj, void *owner, void *holder, s32 mode, void *desc, s32 flag);
extern s32 func_800957C0(Work *work, void *out, s32 a2, void *a3, s32 a4);
extern Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);
/* Struct return: the caller passes the hidden result pointer, as for func_8009AB34. */
extern Context func_80097F20(void *menu);
extern void *func_80093944(void *unused, s32 type, Context *ctx, void *source);
extern s32 func_800D0248(void *object);
extern void *func_80093530(void *unused, s32 type, Context *ctx);
void *func_80093398(Object *self) {
    s32 choices[8];
    Context ctx;
    Context temp;
    Work *work;
    s32 i;
    s32 selected;
    Obj800EBA54 *actor=D_801476B8;
    Member800EBA54 *owner;
    for(i=7;i>=0;i--) choices[i]=0;
    owner=func_800EBA54(actor);
    work=&D_801404E0;
    func_80097B90(work,owner,0,2,D_80138F98,0);
    selected=func_800957C0(work,choices,1,self->selection_64,0)==1;
    if (!selected) return 0;
    func_800D0180(&ctx);
    if (choices[1]==0x43) {
        work=&D_80141104;
        temp=func_80097F20(work);
        ctx=temp;
        choices[1]=choices[3];
    } else {
        temp=func_80097F20(work);
        ctx=temp;
    }
    if (choices[1]==0x15) {
        s32 available=0;
        work=&D_801408EC;
        if (work->field_2CC!=0) available=work->field_2D0>0;
        if (available) choices[1]=0x2A;
    }
    if (choices[1]>=0x28) {
        if (choices[1]<0x2B || choices[1]==0x2C)
            return func_80093944(self,choices[1],&ctx,work);
    }
    selected=func_800D0248(&ctx)==1;
    if (!selected) return 0;
    return func_80093530(self,choices[1],&ctx);
}
