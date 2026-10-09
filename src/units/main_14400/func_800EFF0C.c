#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 bytes[4]; } PackedAttributes;
typedef struct { u8 bytes[16]; } PackedStats;
typedef struct { u16 field_00, field_02, field_04; u8 flags_06; } Attributes;
typedef struct { u16 field_00, field_02, field_04; u8 field_06, level_07; PackedStats stats_08; } Stats;
typedef struct { short delta, index; void (*fn)(void *, s32); } Destroy;
typedef struct { short delta, index; void (*fn)(void *); } Update;
typedef struct { u8 reserved_00[8]; Destroy destroy; u8 reserved_10[0x50]; Update update; } Methods;
typedef struct { u8 reserved_00[0xA]; u8 id_0A; u8 reserved_0B[0x11]; u16 flags_1C; u8 reserved_1E[6]; Methods *methods; u16 field_28, field_2A, field_2C, field_2E, field_30; u8 reserved_32[0x46]; PackedAttributes attributes_78; PackedStats stats_7C; u8 reserved_8C[4]; u32 flags_90; u16 field_94, field_96, field_98; } Object;
extern u32 D_8013960C;
extern void *func_80044ECC(u8 kind, u8 level);
extern void *func_80044F5C(u8 arg0, u8 arg1);
extern char *func_80044FDC(u8 a, u8 b);
extern s32 func_800E0F40(Object *);
extern void func_800E039C(Object *object, s32 value);
void func_800EFF0C(Object *self) {
    u8 id = self->id_0A;
    s32 level = func_800E0F40(self);
    PackedAttributes *attributes = func_80044ECC(id, (u8)level);
    if (!attributes) {
        if (self) {
            self->methods->destroy.fn((u8 *)self + self->methods->destroy.delta, 3);
        }
    } else {
        Attributes *entry;
        Stats *stats;
        u16 flags;
        u8 kind;
        self->attributes_78 = *attributes;
        D_8013960C <<= 1;
        entry = func_80044F5C(id, (u8)level);
        self->field_94 = entry->field_00;
        self->field_96 = entry->field_02;
        self->field_98 = entry->field_04;
        flags = self->flags_1C & 0xFF83;
        self->flags_1C = flags;
        kind = entry->flags_06;
        if (kind & 0x80) self->flags_1C = flags | 0x40;
        switch (kind & 7) {
        case 1: self->flags_1C |= 4; break;
        case 2: self->flags_1C |= 8; break;
        case 3: self->flags_1C |= 0x10; break;
        case 4: self->flags_1C |= 0x20; break;
        case 5: self->flags_90 |= 0x1000000; break;
        }
        self->flags_1C |= 0x80;
        stats = (Stats *)func_80044FDC(id, (u8)level);
        self->field_28 = stats->field_00;
        self->field_2A = stats->field_00;
        self->field_2C = stats->field_02;
        self->field_2E = stats->field_02;
        self->field_30 = stats->field_04;
        func_800E039C(self, stats->level_07);
        self->stats_7C = stats->stats_08;
        self->methods->update.fn((u8 *)self + self->methods->update.delta);
        D_8013960C >>= 1;
    }
}
