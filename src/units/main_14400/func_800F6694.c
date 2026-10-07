#include "common.h"
typedef struct { char pad[0x14]; s32 unk14; } Sub;
typedef struct { char pad[0x80]; Sub *unk80; } Obj;
s32 func_800F438C(Obj *, void *event);
void func_800F6694(Obj *self, void *event) {
    func_800F438C(self, event);
    self->unk80->unk14 = 1;
}
