#include "common.h"

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
typedef MenuPanel Panel8009A3C4;

typedef struct {
    const void *vtbl;
    char unknown4[0xC];
    s32 textHandle10;
} Text8009A3C4;

typedef struct {
    char pad0[0x4C];
    const void *vtbl;
    char pad50[0x5C - 0x50];
    Text8009A3C4 unk5C;
} Head8009A3C4;

extern Head8009A3C4 D_80141A28;
typedef struct ObjA8009A514 {
    unsigned char pad_00[0x4C];
    const void *vtable_4C;
    unsigned char pad_50[0x78 - 0x50];
} ObjA8009A514;
typedef struct ObjB8009A514 {
    unsigned char pad_00[0x4C];
    const void *vtable_4C;
    unsigned char pad_50[0x60 - 0x50];
    s32 index_60;
    void *data_64;
    unsigned char pad_68[0x94 - 0x68];
} ObjB8009A514;
extern ObjA8009A514 D_8014191C;
extern ObjB8009A514 D_80141994;
extern Panel8009A3C4 D_80141510;
extern Panel8009A3C4 D_80141104;
extern Panel8009A3C4 D_80140CF8;
extern Panel8009A3C4 D_801408EC;
extern Panel8009A3C4 D_801404E0;
extern char D_8014A9E8[];
extern const unsigned char D_80151E38[144];

void func_80095010(Text8009A3C4 *arg0, s32 arg1);
void func_800951D0(char *arg0, s32 arg1);

static inline void func_8009A3C4_head(Head8009A3C4 *head) {
    func_80095010(&head->unk5C, 2);
    head->vtbl = D_80151E38;
}

static inline void func_8009A3C4_init(Panel8009A3C4 *panel) {
    MenuBase *second;

    second = &panel->second;
    panel->base.vtbl = D_8014A9E8;
    panel->unk2A0 = D_80151E38;
    panel->unk194 = D_80151E38;
    func_800951D0(panel->unk128, 2);
    second->vtbl = D_80151E38;
    panel->base.vtbl = D_80151E38;
}

void func_8009A3C4(void) {
    func_8009A3C4_head(&D_80141A28);
    D_80141994.vtable_4C = D_80151E38;
    D_8014191C.vtable_4C = D_80151E38;
    func_8009A3C4_init(&D_80141510);
    func_8009A3C4_init(&D_80141104);
    func_8009A3C4_init(&D_80140CF8);
    func_8009A3C4_init(&D_801408EC);
    func_8009A3C4_init(&D_801404E0);
}
