#include "common.h"

typedef struct {
    unsigned char unk00[0x24];
    const void *unk24;
} Object;
extern const unsigned char D_8015C148[192];
extern void func_800EFD28(Object *, s32);
extern void func_800A3918(Object *);

void func_80106F6C(Object *object, s32 flags) {
    object->unk24 = D_8015C148;
    func_800EFD28(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
