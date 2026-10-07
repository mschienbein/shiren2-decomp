#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[8];
    u8 unk8;
} Obj8006C3D8;

u32 func_80031F90(u32 mask);

void func_8006C3D8(Obj8006C3D8 *obj, u8 value) {
    u32 saved = func_80031F90(1);

    obj->unk8 = value;
    func_80031F90(saved);
}
