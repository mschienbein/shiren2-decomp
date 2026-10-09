#include "common.h"
/* Complete 0x8C-byte static menu, ending before D_80141B5C. */
typedef struct { char pad[0x4C]; void *vtbl; char pad50[0xC]; s32 unk5C; const void *unk60; char pad64[0x28]; } Obj8009C1F8;
extern Obj8009C1F8 D_80141AD0;
extern s32 D_80152800[];
extern const s32 D_80151EC8[];
Obj8009C1F8 *func_800953C0(Obj8009C1F8 *);
void func_8009C1F8(void) {
    Obj8009C1F8 *obj = &D_80141AD0;
    func_800953C0(obj);
    obj->vtbl = D_80152800;
    obj->unk5C = -1;
    obj->unk60 = D_80151EC8;
}
