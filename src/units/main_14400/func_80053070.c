#include "common.h"
typedef short s16;
typedef unsigned char u8;
/* Twelve-byte animation records; the initializer writes through offset 9. */
typedef struct { void *unk0; s16 unk4; u8 unk6, unk7, unk8, unk9; } Slot;
extern Slot D_801616D0[2];
extern Slot D_801616E8;
extern Slot D_801616F4;
void *func_80052878(void);
void *func_80052888(void);
void *func_80052F54(s16 key);
void func_80053590(Slot *);
void func_80053400(void);
void func_800535A4(void);
void func_80053070(void) {
    Slot *slot = D_801616D0;
    Slot *a = &D_801616E8;
    Slot *b = &D_801616F4;
    a->unk0 = func_80052878();
    b->unk0 = func_80052888();
    slot->unk0 = func_80052F54(0x1A5);
    slot++;
    slot->unk0 = func_80052F54(0x1A5);
    func_80053590(a);
    func_80053590(b);
    func_80053400();
    func_800535A4();
}
