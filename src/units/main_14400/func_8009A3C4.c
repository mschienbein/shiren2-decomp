#include "common.h"

typedef struct {
    char pad0[0x4C];
    void *vtbl;
} Base8009A3C4;

typedef struct {
    Base8009A3C4 base;
    char pad50[0xC8 - 0x50];
    Base8009A3C4 second;
    char pad118[0x128 - 0x118];
    char unk128[0x194 - 0x128];
    void *unk194;
    char pad198[0x2A0 - 0x198];
    void *unk2A0;
} Panel8009A3C4;

typedef struct {
    void *vtbl;
    char unknown4[0xC];
    s32 textHandle10;
} Text8009A3C4;

typedef struct {
    char pad0[0x4C];
    void *vtbl;
    char pad50[0x5C - 0x50];
    Text8009A3C4 unk5C;
} Head8009A3C4;

extern Head8009A3C4 D_80141A28;
extern void *D_801419E0;
extern void *D_80141968;
extern Panel8009A3C4 D_80141510;
extern Panel8009A3C4 D_80141104;
extern Panel8009A3C4 D_80140CF8;
extern Panel8009A3C4 D_801408EC;
extern Panel8009A3C4 D_801404E0;
extern char D_8014A9E8[];
extern char D_80151E38[];

void func_80095010(Text8009A3C4 *arg0, s32 arg1);
void func_800951D0(char *arg0, s32 arg1);

static inline void func_8009A3C4_head(Head8009A3C4 *head) {
    func_80095010(&head->unk5C, 2);
    head->vtbl = D_80151E38;
}

static inline void func_8009A3C4_init(Panel8009A3C4 *panel) {
    Base8009A3C4 *second;

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
    D_801419E0 = D_80151E38;
    D_80141968 = D_80151E38;
    func_8009A3C4_init(&D_80141510);
    func_8009A3C4_init(&D_80141104);
    func_8009A3C4_init(&D_80140CF8);
    func_8009A3C4_init(&D_801408EC);
    func_8009A3C4_init(&D_801404E0);
}
