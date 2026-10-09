#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Loader Loader;
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *obj, s32 index);
} VtblEntry8009FB28;
typedef struct {
    u8 pad00[0x68];
    VtblEntry8009FB28 slot68;
} Vtbl8009FB28;
struct Loader {
    u8 pad00[0x20];
    s32 row20;
    u8 pad24[0x34 - 0x24];
    s32 base34;
    s32 stride38;
    u8 pad3C[0x4C - 0x3C];
    Vtbl8009FB28 *vtable;
    u8 pad50[0x124 - 0x50];
    s32 active124;
    s32 busy128;
    u8 slots12C[0x140 - 0x12C];
};

extern s32 func_8009F8F8(Loader *loader, s32 index);

s32 func_8009FB28(Loader *loader, s32 index)
{
    if (loader->active124 != 0 && loader->busy128 == 0) {
        if (index != 0) {
            return -1;
        }
        return func_8009F8F8(loader,
            loader->vtable->slot68.fn((u8 *)loader + loader->vtable->slot68.delta,
                                      loader->base34 + loader->row20 * loader->stride38));
    }
    return func_8009F8F8(loader, loader->slots12C[index]);
}
