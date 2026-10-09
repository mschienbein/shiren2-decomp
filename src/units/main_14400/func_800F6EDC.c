#include "common.h"

typedef struct {
    unsigned char unk00[0x20];
    short unk20;
    s32 (*unk24)(void *);
} VTable;
typedef struct { s32 unk00; VTable *unk04; } Part;
typedef struct {
    unsigned char unk00[0x8C];
    Part unk8C;
    unsigned char unk94[0x14];
    s32 unkA8;
} Object;
extern u32 D_8013960C;
extern s32 func_800E0F40(Object *);

void func_800F6EDC(Object *object) {
    s32 active;
    Part *part;
    VTable *vtable;
    D_8013960C *= 2;
    active = 0;
    if ((u32)(func_800E0F40(object) & 0xFF) >= 2) {
        part = &object->unk8C;
        vtable = part->unk04;
        active = vtable->unk24((unsigned char *)part + vtable->unk20) != 0;
    }
    object->unkA8 = active;
    D_8013960C >>= 1;
}
