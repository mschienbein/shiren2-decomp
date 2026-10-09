#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* ROM records contain segmented pointers until this routine relocates the pointer slots. */
typedef struct { u8 flags; u8 pad1[3]; void *field_4; u8 count; u8 pad9[3]; void *field_C; } Resource;
typedef struct { u8 pad0[6]; u16 field_6; } Metadata;
typedef struct { Metadata *field_0; u8 field_4, field_5; u8 pad6[2]; } Entry_80074784;
typedef struct {
    u8 pad0[2]; short kind; u8 pad4[2]; u8 level, mode, pad8, state; u16 flags;
    u8 padC[0x30]; u16 resourceId; u8 field_3E, pad3F, field_40; u8 pad41[0xB];
    Resource *field_4C, *field_50; void *field_54, *field_58; u8 pad5C[0x54];
} Entity;
extern Entity D_801DEAB4[];
extern const u8 D_5000000[], D_00E53DB0[];
Entry_80074784 *func_80074784(s32 kind, s32 level);
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_8007482C(s32 index, s32 value);
s32 func_80041FF8(void);
void func_80074BF4(s32 index, s32 mode, s32 randomize);
s32 func_80076044(s32 index, s32 resource, s32 mode, s32 value, s32 enabled) {
    Entry_80074784 *entry;
    s32 changed = 0;
    s32 randomize = 0;
    Entity *self = &D_801DEAB4[index];
    if (self->kind == -1 || !self->field_50 || !self->field_58 || !self->field_54) return -1;
    if (self->resourceId != resource) {
        entry = func_80074784(self->kind, self->level);
        if (!resource) { changed = 1; resource = entry->field_0->field_6; }
        self->resourceId = resource;
        /* local-arithmetic-qualification: ROM symbols and segmented pointers are in a different address space from RAM; in-bounds CPU pointer arithmetic cannot express the integer cartridge address required by DMA. */
        func_8006AAF0(self->field_50, ((u32)D_5000000 & 0xFFFFFF) + (u32)D_00E53DB0 + resource * 16, 16);
        func_8006AAF0(self->field_58, ((u32)self->field_50->field_C & 0xFFFFFF) + (u32)D_00E53DB0, self->field_50->count * 12);
        func_8006AAF0(self->field_54, ((u32)self->field_50->field_4 & 0xFFFFFF) + (u32)D_00E53DB0, (self->field_50->flags & 31) * 4);
        self->field_50->field_C = self->field_58;
        self->field_50->field_4 = self->field_54;
        self->field_4C = self->field_50;
        func_8007482C(index, entry->field_5 ? entry->field_5 : self->level);
    }
    if (self->mode != mode) changed = 1;
    if ((u32)(self->state - 3) < 5 && !(func_80041FF8() & 0xFF)) {
        if (!(self->flags & 0x4000)) randomize = 1;
    }
    func_80074BF4(index, changed, randomize);
    if (mode == 2) self->field_3E = value;
    self->mode = mode;
    self->field_40 = enabled;
    return 0;
}
