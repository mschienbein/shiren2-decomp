#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct { f32 x, y, z; } Vec3f;
/* Fixed-point (gu) matrix. */
typedef struct { s32 m[4][4]; } Mtx;
typedef union { u32 word; u8 rgba[4]; } Color;

/* 0x54-byte display record (layout shared with func_80070A0C/func_80070D30). */
typedef struct Model {
    u8 kind;          /* 0x00 */
    u8 flags;         /* 0x01 */
    u8 field2;        /* 0x02 */
    u8 pad3;
    u32 field4;       /* 0x04 */
    u32 draw8;        /* 0x08 */
    u32 modeC;        /* 0x0C */
    u32 mode10;       /* 0x10 */
    Color color14;    /* 0x14 */
    Color env18;      /* 0x18 */
    u8 combine1C[16]; /* 0x1C */
    Mtx *matrix2C;    /* 0x2C */
    Mtx *matrix30;    /* 0x30: optional second matrix (func_8006F250 emits it as 0xDA380001) */
    f32 field34;      /* 0x34: view depth */
    u8 field38;
    u8 field39;       /* 0x39 */
    u8 pad3A[2];
    s32 field3C;      /* 0x3C */
    s32 field40;      /* 0x40 */
    s32 field44;      /* 0x44 */
    s32 field48;      /* 0x48 */
    s32 field4C;      /* 0x4C */
    u8 red50;         /* 0x50 */
    u8 green51;       /* 0x51 */
    u8 blue52;        /* 0x52 */
    u8 pad53;
} Model;

/* 12-byte frame of a unit descriptor's pose table. */
typedef struct { u8 pad0[9]; s8 lift_09; s8 step_0A; u8 pad0B; } Pose;
/* ROM descriptors hold segmented addresses until relocated to RAM buffers. */
typedef union { u32 segmented; void *resident; Pose *poses; } TableRef;
typedef struct {
    u8 count_00;      /* low 5 bits: palette levels; 0x20: translucent */
    u8 pad01[3];
    TableRef table_04;
    u8 count_08;
    u8 pad09[3];
    TableRef table_0C;
} Descriptor;

/* 0xB0-byte unit record shared by the five unit tables. */
typedef struct Unit {
    s16 type_00;      /* 0 monster, 1/4 attached, 3 tile object, 4 effect, 5 shadow */
    s16 id_02;        /* -1 = free */
    u8 field_04;
    s8 mode_05;
    u8 level_06;
    u8 active_07;     /* 1 = animation running */
    u8 state_08;
    u8 action_09;
    u16 flags_0A;
    s16 x_0C;
    s16 y_0E;
    s16 z_10;
    u8 side_12;
    u8 slot_13;
    f32 lift_14;
    s32 timer_18;
    f32 scale_1C;
    f32 scale_20;
    f32 scale_24;
    f32 rot_28;
    f32 rot_2C;
    f32 rot_30;
    s8 bob_34;
    u8 raised_35;
    u8 billboard_36;
    u8 attach_37;
    f32 speed_38;
    u16 sprite_3C;
    u8 frame_3E;
    u8 frame_3F;
    u8 frameStep_40;
    u8 palette_41;
    u8 step_42;
    s8 timer_43;
    u8 show_44;
    u8 slot_45;
    union { u16 pair; u8 v[2]; } fade46; /* v[0] alpha, v[1] fade */
    u8 bright_48;
    u8 flags_49;
    s16 dir_4A;
    Descriptor *desc_4C;
    Descriptor *buf_50;
    void *buf_54;
    void *buf_58;
    Model model;      /* 0x5C */
} Unit;

/* Animation frame; id -1 marks a jump to frame `arg` (0 when arg is -1),
 * otherwise `arg` is the frame duration. */
typedef struct { s8 id; s8 arg; s16 value; } AnimFrame;
typedef struct { s32 count; const AnimFrame *frames; } Anim;
typedef struct { s8 value; s8 next; u8 pad[2]; } Step;
typedef struct { s32 count; const Step *steps; } Sequence;

/* One map cell of a movement path: tile coordinates plus a state flag. */
typedef struct { s16 x; s16 y; s16 state; } Cell;
/* 0x2E-byte movement group record; a negative count marks an overflowed group. */
typedef struct {
    s8 count;
    u8 pad_01;
    Cell from[3];
    Cell to[3];
    s16 anim[4];      /* [3]: last value */
} Zone;

typedef struct { s32 cur; s32 rep; } Repeat;
typedef struct { Repeat menu; Repeat shoulder; Repeat cbtn; } PadRepeat;

/* 12-byte effect pair record: ids[0] the 0x162 unit, ids[1] the 0x13A unit. */
typedef struct { u8 active; u8 timer; u8 pad2[2]; s32 ids[2]; } EffectPair;

typedef struct {
    s16 weapon_00;
    s16 unk_02;
    s16 shield_04;
    s16 state_06;
    s16 weaponFlag_08;
    s16 shieldFlag_0A;
} Equip;

/* 8-byte species record (D_8013FA90). */
typedef struct {
    u16 scale_00;
    u8 size_02;
    u8 anim_03;
    u8 height_04;
    s8 lift_05;
    u16 anim_06;
} Species;
typedef struct { Species *species; u8 index; u8 palette; u8 pad6[2]; } SpeciesRef;

extern Unit D_801DEAB4[30];
extern Unit D_801D8FFC[4];
extern Unit D_801D2C2C[30];
extern Unit D_801E02A8[110];
extern Unit D_801DD378[32];
extern Unit *D_801A76B0[];
extern Zone D_801A79E8[30];
extern PadRepeat D_801A7320[30];
extern EffectPair D_801A75F0[16];
extern Descriptor *D_801D25A0[];
/* ORIGINAL BUG: the two pre-flag sorter calls in func_8007A680 pass the
 * vacant ID -1. The original lookup forms 0x8013F3C8, eight bytes before
 * this array; no same-array negative-prefix owner is established. Keep
 * the declared origin rather than enlarging storage to hide the defect.
 * On the inspected fresh-builder frame path, both results are discarded
 * without a table read. Evidence: scratch/omp/wave37/species-reachability-evidence/
 * source-context-report.json R1-R4 and species-storage-evidence/report.json.
 * This documents that exact pointer-formation case, not a valid-ID guarantee. */
extern SpeciesRef D_8013F3D0[];
extern SpeciesRef D_8013FE90[];
extern SpeciesRef D_8013FEB0[];
extern SpeciesRef D_8013FEC8[];
extern Species D_8013FA90[];
extern const u8 D_5000000[], D_00E53DB0[];

/* .data 0x8013D890-0x8013D953 */
u32 D_8013D890[9] = { 0x1, 0x2, 0x4, 0x8, 0x200, 0x80, 0x100, 0x400, 0x800 };
u32 D_8013D8B4[2] = { 0x20, 0x40 };
u32 D_8013D8BC[1] = { 0x10 };
/* Display-record cursors reset here each frame. */
Model *D_8013D8C0 = 0;
Model *D_8013D8C4 = 0;
s32 D_8013D8C8 = 0;
s32 D_8013D8CC = 0;
s32 D_8013D8D0 = 0;
s32 D_8013D8D4 = 0;
s32 D_8013D8D8 = 0;
s32 D_8013D8DC = 0;
s32 D_8013D8E0 = 0;
s32 D_8013D8E4 = 1;
s32 D_8013D8E8 = 0;
s32 D_8013D8EC = 1;
s32 D_8013D8F0 = 0;
s32 D_8013D8F4 = 0;
s32 D_8013D8F8 = 0;
s32 D_8013D8FC = 0;
s32 D_8013D900 = 1;
Equip D_8013D904[2] = { { -1, -1, 0, 0, 0, 0 }, { -1, -1, 0, 0, 0, 0 } };
u8 D_8013D91C[4] = { 0, 0, 0, 0x80 };
s32 D_8013D920 = 0xC8;
s32 D_8013D924[9] = { 0x1E, 0x800, 0x20, 0x8, 0x2000, 0x20, 0x4, 0x900, 0x20 };
s32 D_8013D948[3] = { 0x20, 0x400, 0x20 };

extern const u8 D_8014CC80[], D_8014CCA4[], D_8014CCC8[], D_8014CCEC[], D_8014CD10[], D_8014CD34[],
    D_8014CD58[], D_8014CD7C[], D_8014CDA0[], D_8014CDC4[], D_8014CDE8[], D_8014CE0C[];
extern const Anim D_8014CED8, D_8014CEE0, D_8014CEE8, D_8014CEF0, D_8014CEF8, D_8014CF00, D_8014CF08;
extern const Sequence D_8014CF34, D_8014CF3C, D_8014CF64, D_8014CF6C;
extern const Anim D_8014CF9C, D_8014CFA4;
extern const Anim D_8014D1B4[19];

SpeciesRef *func_80074784(s32 kind, s32 level);
extern s32 func_80041C64(s32 value);
Unit *func_8007946C(s32 side, s32 slot);
void func_80074778(Unit *unit);
s32 func_80074500(Unit *units, s32 index, s32 count, s32 kind, s32 resource);
void func_80074D88(Unit *unit, const Anim *anim, s32 dt);
void func_80074E70(Unit *unit, const Sequence *seq, s32 elapsed);
void func_80079800(f32 m[4][4], f32 x, f32 y, f32 z);
Zone *func_80079448(s32 index);
void func_800593F8(Vec3f *pos);
void func_8002CE80(f32 mf[4][4], Mtx *m);
void func_8002CD40(f32 mf[4][4], Mtx *m);

/* ==== func_80074040 ==== */
extern void *func_8006A8D8(char *name, u32 size);

void *func_80074040(void *name, s32 size)
{
    void *result;

    result = func_8006A8D8(name, size);
    if (result == 0) {
        D_8013D8CC = 0;
    }
    return result;
}

/* ==== func_8007406C ==== */
void func_8007406C(s32 arg) {
    D_8013D8D4 = arg;
}

/* ==== func_80074078 ==== */
void func_80074078(s32 arg)
{
    D_8013D8D0 = arg;
}

/* ==== func_80074084 ==== */
void func_80074084(s32 arg)
{
    D_8013D8E0 = arg;
}

/* ==== func_80074090 ==== */
void func_80074090(s32 arg)
{
    D_8013D8D8 = arg;
}

/* ==== func_8007409C ==== */
void func_8007409C(s32 value)
{
    D_8013D8DC = value;
}

/* ==== func_800740A8 ==== */
void func_800740A8(s32 value) { D_8013D8E8 = value; }

/* ==== func_800740B4 ==== */
void func_800740B4(s32 arg)
{
    D_8013D8F0 = arg;
}

/* ==== func_800740C0 ==== */
void func_800740C0(s32 arg) {
    D_8013D8F4 = arg;
}

/* ==== func_800740CC ==== */
void func_800740CC(s32 arg) {
    D_8013D8F8 = arg;
}

/* ==== func_800740D8 ==== */
void func_800740D8(s32 value) {
    D_8013D8FC = value;
}

/* ==== func_800740E4 ==== */
void func_800740E4(s32 arg) {
    D_8013D900 = arg;
}

/* ==== func_800740F0 ==== */
void func_80074174(s32);
void func_800740F0(void) { D_8013D8CC = 0; func_80074174(1); }

/* ==== func_80074114 ==== */
extern s32 func_8005B07C(void);
s32 func_80074114(void) {
    s32 value = func_8005B07C();
    if (value < 0) return 0;
    return value;
}
/* ==== func_80074140 ==== */

s32 func_80074140(void) {
    if (D_8013D8CC == 0) {
        return -1;
    }
    func_80074174(0);
    return 0;
}

/* ==== func_80074174 ==== */
void func_800765AC(void);
void func_8007BDE4(void);

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

static inline void resetActors(Unit *actor, s32 count, s32 full) {
    Unit *last = &actor[count - 1];

    for (; actor <= last; actor++) {
        actor->id_02 = -1;
        if (full) {
            actor->buf_50 = 0;
            actor->buf_58 = 0;
            actor->buf_54 = 0;
        }
    }
}

void func_80074174(s32 full) {
    PadRepeat *slot;
    PadRepeat *last;

    resetActors(D_801DEAB4, ARRAY_COUNT(D_801DEAB4), full);
    resetActors(D_801D8FFC, ARRAY_COUNT(D_801D8FFC), full);
    resetActors(D_801D2C2C, ARRAY_COUNT(D_801D2C2C), full);
    resetActors(D_801DD378, ARRAY_COUNT(D_801DD378), full);
    resetActors(D_801E02A8, ARRAY_COUNT(D_801E02A8), full);
    slot = D_801A7320;
    last = &slot[ARRAY_COUNT(D_801A7320) - 1];
    for (; slot <= last; slot++) {
        slot->menu.cur = -1;
        slot->shoulder.cur = -1;
        slot->cbtn.cur = -1;
        slot->menu.rep = 0;
        slot->shoulder.rep = 0;
        slot->cbtn.rep = 0;
    }
    func_800765AC();
    func_8007BDE4();
    func_80074084(0);
    func_8007406C(1);
    func_80074078(0);
    func_80074090(0);
    func_800740A8(0);
    func_800740B4(0);
    func_800740C0(0);
    func_800740CC(0);
    func_800740E4(1);
}
/* ==== func_80074374 ==== */

extern s32 func_800718CC(u32, void *, void *);
void func_80074374(void)
{
    s32 i;
    func_80074174(1);
    D_8013D8CC = 1;
    for (i = 0; i < 30; i++) {
        Unit *entry = &D_801DEAB4[i];
        entry->buf_50 = func_80074040("Sprite Index Buff", 0x10);
        entry->buf_58 = func_80074040("Sprite Pattern Info Buff", 0x480);
        entry->buf_54 = func_80074040("Sprite Palette Info Buff", 0x24);
    }
    for (i = 0; i < 4; i++) {
        Unit *entry = &D_801D8FFC[i];
        entry->buf_50 = func_80074040("Sprite Index Buff", 0x10);
        entry->buf_58 = func_80074040("Sprite Pattern Info Buff", 0xCC);
        entry->buf_54 = func_80074040("Sprite Palette Info Buff", 0x10);
    }
    for (i = 0; i < 32; i++) {
        Unit *entry = &D_801DD378[i];
        entry->buf_50 = func_80074040("Sprite Index Buff", 0x10);
        entry->buf_58 = func_80074040("Sprite Pattern Info Buff", 0x480);
        entry->buf_54 = func_80074040("Sprite Palette Info Buff", 0x10);
    }
    if (func_800718CC(3, D_8013D924, D_8013D948)) D_8013D8CC = 0;
}
/* ==== func_800744DC ==== */

void func_800744DC(void) { func_80074174(1); D_8013D8CC = 0; }

/* ==== func_80074500 ==== */
extern u32 func_8002A9B0(void);
extern void func_8006AAF0(void *, u32, s32);
s32 func_80074500(Unit *actors, s32 index, s32 count, s32 kind, s32 resource) {
    Unit *actor;
    Descriptor *descriptor;
    if (!D_8013D8CC) return -1;
    if (count) {
        s32 end = index + count;
        for (; index < end; index++) if (actors[index].id_02 == -1) break;
        if (index >= end) return -1;
    }
    actor = &actors[index];
    actor->type_00 = 0;
    actor->id_02 = kind;
    actor->field_04 = 0; actor->mode_05 = 0; actor->level_06 = 0; actor->active_07 = 0; actor->state_08 = 0; actor->action_09 = 0;
    actor->flags_0A = 0; actor->x_0C = 0; actor->y_0E = 0; actor->z_10 = 0;
    actor->lift_14 = 0;
    actor->timer_18 = func_8002A9B0() & 0x1F;
    actor->dir_4A = 0;
    actor->scale_1C = 1.0f; actor->scale_20 = 1.0f; actor->scale_24 = 1.0f;
    actor->rot_28 = 0; actor->rot_2C = 0; actor->rot_30 = 0;
    actor->bob_34 = 0; actor->raised_35 = 0; actor->billboard_36 = 3; actor->attach_37 = 0;
    actor->side_12 = 0; actor->slot_13 = 0; actor->speed_38 = 0;
    actor->sprite_3C = resource;
    actor->frame_3E = 0; actor->frame_3F = 0; actor->frameStep_40 = 0; actor->step_42 = 0; actor->timer_43 = 0; actor->palette_41 = 0; actor->show_44 = 0; actor->slot_45 = 0;
    actor->model.color14.rgba[0] = 0xFF; actor->model.color14.rgba[1] = 0xFF; actor->model.color14.rgba[2] = 0xFF;
    actor->model.color14.rgba[3] = 0; actor->model.env18.rgba[0] = 0; actor->model.env18.rgba[1] = 0; actor->model.env18.rgba[2] = 0; actor->model.env18.rgba[3] = 0;
    actor->fade46.v[0] = 0xFF; actor->fade46.v[1] = 0xFF; actor->bright_48 = 0; actor->flags_49 = 0;
    descriptor = D_801D25A0[resource];
    if (descriptor) actor->desc_4C = descriptor;
    else {
        if (!actor->buf_50 || !actor->buf_58 || !actor->buf_54) return -1;
        /* The DMA API takes encoded ROM device addresses, not dereferenceable CPU pointers. */
        func_8006AAF0(actor->buf_50, (u32)D_00E53DB0 + ((u32)D_5000000 & 0xFFFFFF) + resource * 16, 0x10);
        func_8006AAF0(actor->buf_58, (u32)D_00E53DB0 + (actor->buf_50->table_0C.segmented & 0xFFFFFF), actor->buf_50->count_08 * 12);
        func_8006AAF0(actor->buf_54, (u32)D_00E53DB0 + (actor->buf_50->table_04.segmented & 0xFFFFFF), (actor->buf_50->count_00 & 0x1F) * 4);
        actor->buf_50->table_0C.resident = actor->buf_58;
        actor->buf_50->table_04.resident = actor->buf_54;
        actor->desc_4C = actor->buf_50;
    }
    return index;
}

/* ==== func_80074778 ==== */
void func_80074778(Unit *shadow) { shadow->id_02 = -1; }

/* ==== func_80074784 ==== */
SpeciesRef *func_80074784(s32 kind, s32 level) {
    s32 index = 0;

    if (level > 0) {
        index = level - 1;
    }
    switch (kind) {
        case 0x37:
            if (index >= 4) {
                index = 3;
            }
            return &D_8013FE90[index];
        case 0x31:
            if (index >= 3) {
                index = 2;
            }
            return &D_8013FEB0[index];
        case 0x25:
            if (index >= 3) {
                index = 2;
            }
            return &D_8013FEC8[index];
    }
    /* ORIGINAL BUG: kind==-1 in the two pre-flag sorter calls forms an
     * invalid before-array pointer. ROM 0x477F0/0x4785C..0x47868 computes
     * 0x8013F3D0 + (-1 * 8) without a load; later caller guards discard it.
     * Other consuming calls retain their valid-kind precondition. */
    return &D_8013F3D0[kind];
}

/* ==== func_8007482C ==== */
void func_8007482C(s32 index, s32 value)
{
    Unit *entry = &D_801DEAB4[index];
    s32 level;
    s32 max;
    u8 result;

    if (D_801DEAB4[index].id_02 == 0x29) {
        if (value < 20) {
            level = 0;
        } else if (value < 30) {
            level = 1;
        } else if (value < 60) {
            level = 2;
        } else if (value < 70) {
            level = 3;
        } else if (value < 77) {
            level = 4;
        } else if (value < 88) {
            level = 5;
        } else if (value < 90) {
            level = 6;
        } else if (value < 99) {
            level = 7;
        } else {
            level = 8;
        }
    } else {
        level = 0;
        if (value > 0) {
            level = value - 1;
        }
    }

    max = entry->desc_4C->count_00 & 0x1F;
    if (level < max) {
        result = level;
    } else {
        result = max - 1;
    }
    entry->palette_41 = result;
}
/* ==== func_800748F8 ==== */

s32 func_80042734(s32);
s32 func_80076C34(s32, s32, s32);
s32 func_80076044(s32, s32, s32, s32, s32);
void func_80075E5C(s32, s32);
s32 func_800748F8(s32 owner, s32 type, s32 x, s32 z, s32 arg4, s32 variant) {
    SpeciesRef *info = func_80074784(type, variant);
    s32 anim;
    s32 index;
    Unit *actor;
    PadRepeat *set;
    Unit *shadow;
    s32 none;

    if (type == 0x47 && func_80042734(owner) == 2) {
        anim = 0x178;
    } else {
        anim = info->species->anim_06;
    }
    index = func_80074500(D_801DEAB4, owner, 0, type, anim);
    if (index == -1) {
        return -1;
    }
    actor = &D_801DEAB4[index];
    actor->x_0C = (x << 7) + 0x40;
    actor->type_00 = 0;
    actor->level_06 = variant;
    actor->raised_35 = 1;
    actor->z_10 = (z << 7) + 0x40;
    actor->dir_4A = arg4;
    actor->show_44 = 1;
    actor->slot_45 = index;
    actor->scale_1C = info->species->size_02 / 100.0f;
    actor->scale_20 = info->species->size_02 / 100.0f;
    actor->scale_24 = info->species->size_02 / 100.0f;
    actor->speed_38 = info->species->scale_00 / 100.0f;
    actor->y_0E = func_80076C34(index, 2, 0);
    switch (type) {
    case 0x1C:
    case 0x49:
    case 0x53:
    case 0x56:
        actor->billboard_36 = 4;
        break;
    case 0x1E:
        actor->billboard_36 = 4;
        break;
    }
    if (info->palette != 0) {
        func_8007482C(index, info->palette);
    } else {
        func_8007482C(index, variant);
    }
    none = -1;
    func_80076044(index, anim, 1, 8, 3);
    func_80075E5C(index, 0);
    {
        Unit *shadows = D_801D2C2C;

        shadow = &shadows[index];
    }
    if (shadow->id_02 != none) {
        func_80074778(shadow);
    }
    set = &D_801A7320[index];
    set->menu.cur = none;
    set->shoulder.cur = none;
    set->cbtn.cur = none;
    set->menu.rep = 0;
    set->shoulder.rep = 0;
    set->cbtn.rep = 0;
    return index;
}

/* ==== func_80074B90 ==== */
void func_80074B90(s32 index) {
    Unit *shadows = D_801D2C2C;
    Unit *self = &shadows[index];
    if (self->id_02 != -1) func_80074778(self);
    func_80074778(&D_801DEAB4[index]);
}
/* ==== func_80074BF4 ==== */

void func_80074BF4(s32 index, s32 mode, s32 randomize) {
    Unit *actor = &D_801DEAB4[index];
    SpeciesRef *info = func_80074784(actor->id_02, actor->level_06);
    s32 anim;
    const Anim *table;

    if (actor->id_02 == 0x17) {
        switch (actor->action_09) {
        case 2:
            anim = 0xF;
            break;
        case 1:
            anim = 0xE;
            break;
        default:
            anim = info->species->anim_03;
            break;
        }
    } else {
        anim = info->species->anim_03;
    }
    if (actor->mode_05 == 1) {
        mode = 1;
        randomize = 0;
        switch (actor->id_02) {
        case 0x17:
            anim = 0x10;
            break;
        case 0x1B:
            anim = 0x12;
            break;
        }
    }
    table = &D_8014D1B4[anim];
    if (mode == 1) {
        if (randomize == mode) {
            actor->step_42 = func_8002A9B0() % (table->count - 1);
        } else {
            actor->step_42 = 0;
        }
        actor->frame_3E = table->frames[actor->step_42].id;
        actor->timer_43 = table->frames[actor->step_42].arg;
        actor->lift_14 = table->frames[actor->step_42].value;
        actor->step_42++;
    }
}

/* ==== func_80074D88 ==== */
/* Advance the animator by dt ticks and latch the next frame when its timer expires. */
void func_80074D88(Unit *obj, const Anim *anim, s32 dt) {
    u8 index;

    if (obj->active_07 != 1) {
        return;
    }
    if (obj->timer_43 > 0) {
        if (dt < obj->timer_43) {
            obj->timer_43 -= dt;
        } else {
            obj->timer_43 = 0;
        }
    }
    if (obj->timer_43 != 0) {
        return;
    }
    index = obj->step_42;
    if (anim->frames[index].id == -1 || index >= anim->count) {
        if (anim->frames[index].arg == -1) {
            obj->step_42 = 0;
        } else {
            obj->step_42 = anim->frames[index].arg;
        }
    }
    obj->lift_14 = anim->frames[obj->step_42].value;
    obj->frame_3E = anim->frames[obj->step_42].id;
    obj->timer_43 = anim->frames[obj->step_42].arg;
    obj->step_42++;
}

/* ==== func_80074E70 ==== */
void func_80074E70(Unit *p, const Sequence *seq, s32 elapsed) {
    if (p->active_07 == 1) {
        if (p->timer_43 > 0) {
            if (elapsed < p->timer_43) {
                p->timer_43 -= elapsed;
            } else {
                p->timer_43 = 0;
            }
        }
        if (p->timer_43 == 0) {
            if (seq->steps[p->step_42].value == -1 || p->step_42 >= seq->count) {
                if (seq->steps[p->step_42].next == -1) {
                    p->step_42 = 0;
                } else {
                    p->step_42 = seq->steps[p->step_42].next;
                }
            }
            p->palette_41 = seq->steps[p->step_42].value;
            p->timer_43 = seq->steps[p->step_42].next;
            p->step_42++;
        }
    }
}

/* ==== func_80074F34 ==== */
/* Both original callers supply reset_mode (func_800751B4 passes 1, func_80075410 passes 0);
 * this body never inspects it. */
void func_80074F34(s32 pad, s32 reset_mode){
    Unit *c = &D_801DEAB4[pad];
    PadRepeat *p = &D_801A7320[pad];
    s32 i;
    s32 count;
    if ((p->cbtn.rep & 0xF) == 0) {
        if (!(c->flags_0A & 0x10)) {
            p->cbtn.cur = -1;
            p->cbtn.rep = 0;
        } else {
            for (i = 0; i < 1; i++) {
                s32 next = p->cbtn.cur + 1;
                next = (next < 1) ? next : 0;
                p->cbtn.cur = next;
                if (c->flags_0A & D_8013D8BC[next]) break;
            }
        }
    }
    count = 0;
    for (i = 0; i < 1; i++) {
        if (c->flags_0A & D_8013D8BC[i]) count++;
    }
    if (count >= 2) p->cbtn.rep++;
    else p->cbtn.rep = 0;
    if ((p->shoulder.rep & 0xF) == 0) {
        if (!(c->flags_0A & 0x60)) {
            p->shoulder.cur = -1;
            p->shoulder.rep = 0;
        } else {
            for (i = 0; i < 2; i++) {
                s32 t = p->shoulder.cur + 1;
                s32 next = 0;
                if (t < 2) next = t;
                p->shoulder.cur = next;
                if (c->flags_0A & D_8013D8B4[next]) break;
            }
        }
    }
    count = 0;
    for (i = 0; i < 2; i++) {
        if (c->flags_0A & D_8013D8B4[i]) count++;
    }
    if (count >= 2) p->shoulder.rep++;
    else p->shoulder.rep = 0;
    if ((p->menu.rep & 0xF) == 0) {
        if (!(c->flags_0A & 0xF8F)) {
            p->menu.cur = -1;
            p->menu.rep = 0;
        } else {
            for (i = 0; i < 9; i++) {
                s32 t = p->menu.cur + 1;
                s32 next = 0;
                if (t < 9) next = t;
                p->menu.cur = next;
                if (c->flags_0A & D_8013D890[next]) break;
            }
        }
    }
    count = 0;
    for (i = 0; i < 9; i++) {
        if (c->flags_0A & D_8013D890[i]) count++;
    }
    if (count >= 2) p->menu.rep++;
    else p->menu.rep = 0;
}
/* ==== func_800751B4 ==== */

void func_80079560(s32 first, s32 id, s32 mode);
/* Original success and failure exits return 0 and -1. */
s32 func_800751B4(s32 id, s32 flags)
{
    Unit *unit = &D_801DEAB4[id];
    SpeciesRef *entry;
    Unit *shadow;
    if (unit->id_02 == -1) {
        return -1;
    }
    if (unit->id_02 == 0xD6) {
        flags &= 0x8000;
    }
    if (unit->flags_0A == flags) {
        return 0;
    }
    func_80074114();
    entry = func_80074784(unit->id_02, unit->level_06);
    func_80074784(0x17, D_801DEAB4[0].level_06);
    shadow = &D_801D2C2C[id];
    if (shadow->id_02 != -1) {
        func_80074778(shadow);
    }
    unit->model.color14.rgba[0] = 0xFF;
    unit->model.color14.rgba[1] = 0xFF;
    unit->model.color14.rgba[2] = 0xFF;
    unit->model.color14.rgba[3] = 0;
    unit->model.env18.rgba[0] = 0;
    unit->model.env18.rgba[1] = 0;
    unit->model.env18.rgba[2] = 0;
    unit->model.env18.rgba[3] = 0;
    unit->fade46.v[0] = 0xFF;
    unit->scale_1C = entry->species->size_02 / 100.0f;
    unit->scale_20 = entry->species->size_02 / 100.0f;
    func_80079560(0, id, 0);
    unit->flags_0A = flags;
    func_80074F34(id, 1);
    return 0;
}

/* ==== func_8007531C ==== */
/* x/y arrive full width from every caller (mfc1 of trunc.w.s in func_80075410, e.g. 0x8007596C/
 * 0x80075974); the halfword stores to x_0C/y_0E narrow them. */
s32 func_8007531C(s32 index, s32 value, s32 x, s32 y, float first, float second) {
    Unit *source = &D_801DEAB4[index];
    Unit *dest = &D_801D2C2C[index];
    s32 result = func_80074500(D_801D2C2C, index, 0, 0, value);
    if (result == -1) return -1;
    else {
        dest->show_44 = 0xff;
        dest->type_00 = 5;
        dest->attach_37 = 1;
        dest->side_12 = 0;
        dest->slot_13 = index;
        dest->bob_34 = 2;
        dest->active_07 = 1;
        dest->scale_1C = first;
        dest->scale_20 = second;
        dest->x_0C = x;
        dest->y_0E = y;
        dest->fade46.v[0] = source->fade46.v[0];
        dest->fade46.v[1] = source->fade46.v[1];
        return result;
    }
}
/* ==== func_80075410 ==== */

static inline void set_alpha(Unit *p, s32 index, s32 selected, s32 alpha) {
    if (index == 0 || index == func_80041C64(24) || index == func_80041C64(25) || index == func_80041C64(26) || index == func_80041C64(27) || index == func_80041C64(28) || index == selected || (p->flags_0A & 0x880) || D_8013D8F4 == 1) p->fade46.v[0] = alpha;
    else p->fade46.v[0] = 0;
}
static inline SpeciesRef *prepare_source(Unit *source, s32 index) { func_80074F34(index, 0); return func_80074784(source->id_02, source->level_06); }
void func_80075410(void) {
    s32 selected = func_80074114();
    s32 i = 0, alpha;
    s32 inactive = -1;
    for (; i < 30; ++i) {
        Unit *effect = &D_801D2C2C[i];
        Unit *source = &D_801DEAB4[i];
        if (source->id_02 != inactive) {
            SpeciesRef *image, *reference;
            s32 id;
            image = prepare_source(source, i);
            reference = func_80074784(23, D_801DEAB4[0].level_06);
            id = D_801A7320[i].cbtn.cur;
            if (id != inactive && D_8013D8BC[id] == 16) {
                source->model.color14.rgba[0] = 255; source->model.color14.rgba[1] = 255; source->model.color14.rgba[2] = 255;
                source->model.color14.rgba[3] = 0; source->model.env18.rgba[0] = 0; source->model.env18.rgba[1] = 0; source->model.env18.rgba[2] = 0; source->model.env18.rgba[3] = 0;
                if (D_8013D8EC == 1) {
                    s32 value;
                    alpha = (source->timer_18 * 8) % 512;
                    value = alpha < 256 ? alpha & 255 : ~alpha & 255;
                    alpha = (u8)(u32)((float)value * 0.8745098f + 31.0f);
                } else alpha = D_8013D91C[3];
                set_alpha(source, i, selected, alpha);
            }
            if (D_801A7320[i].shoulder.cur != inactive) {
                source->model.color14.rgba[0] = 0; source->model.color14.rgba[1] = 0; source->model.color14.rgba[2] = 0; source->model.color14.rgba[3] = 0; source->model.env18.rgba[3] = 0;
                switch (D_8013D8B4[D_801A7320[i].shoulder.cur]) {
                    case 32: source->model.env18.rgba[0] = 0; source->model.env18.rgba[1] = 128; source->model.env18.rgba[2] = 255; break;
                    case 64: source->model.env18.rgba[0] = 255; source->model.env18.rgba[1] = 255; source->model.env18.rgba[2] = 32; break;
                }
                if (D_8013D8EC == 1) {
                    s32 value;
                    alpha = (source->timer_18 * 8) % 512;
                    value = alpha < 256 ? alpha & 255 : ~alpha & 255;
                    alpha = (u8)(u32)((float)value * 0.8745098f + 31.0f);
                } else alpha = D_8013D91C[3];
                if (source->flags_0A & 16) set_alpha(source, i, selected, alpha);
                else source->fade46.v[0] = 255;
            }
            if (D_801A7320[i].menu.cur != inactive) {
                float ratio = (float)reference->species->size_02 / (float)image->species->size_02;
                switch ((s32)D_8013D890[D_801A7320[i].menu.cur]) {
                    case 1:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 1) || func_8007531C(i, 1, (s32)(ratio * 0.0f), (s32)(ratio * 70.0f), ratio * 1.6f, ratio * 1.6f) != inactive) { effect->fade46.v[0] = source->fade46.v[0]; effect->fade46.v[1] = source->fade46.v[1]; func_80074D88(effect, &D_8014CED8, 1); effect->frame_3F = effect->frame_3E; }
                        break;
                    case 2:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 3) || func_8007531C(i, 3, (s32)(ratio * 40.0f), (s32)(ratio * 40.0f), ratio * 1.6f, ratio * 1.6f) != inactive) { effect->fade46.v[0] = source->fade46.v[0]; effect->fade46.v[1] = source->fade46.v[1]; func_80074D88(effect, &D_8014CEE0, 1); effect->frame_3F = effect->frame_3E; }
                        break;
                    case 4:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 4) || func_8007531C(i, 4, (s32)(ratio * 0.0f), (s32)(ratio * 80.0f), ratio * 1.6f, ratio * 2.2f) != inactive) {
                            u8 opacity; effect->fade46.v[0] = source->fade46.v[0]; opacity = source->fade46.v[1]; effect->frame_3E = 0; effect->frame_3F = 0; effect->fade46.v[1] = opacity;
                        }
                        break;
                    case 8:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 0x135) || func_8007531C(i, 0x135, (s32)(ratio * -4.0f), (s32)(ratio * 86.0f), 2.0f * ratio, 2.0f * ratio) != inactive) { effect->fade46.v[0] = source->fade46.v[0]; effect->fade46.v[1] = source->fade46.v[1]; func_80074D88(effect, &D_8014CEE8, 1); effect->frame_3F = effect->frame_3E; }
                        break;
                    case 0x80:
                    case 0x800:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 0x176) || func_8007531C(i, 0x176, (s32)(ratio * 0.0f), (s32)(ratio * 86.0f), ratio * 3.2f, ratio * 3.2f) != inactive) {
                            effect->fade46.v[0] = (u8)(u32)(((float)source->fade46.v[0] / 255.0f) * 179.0f);
                            effect->fade46.v[1] = source->fade46.v[1];
                            func_80074D88(effect, &D_8014CEF8, 1);
                            effect->frame_3F = effect->frame_3E;
                            if (D_8013D890[D_801A7320[i].menu.cur] == 0x800) effect->palette_41 = 1; else effect->palette_41 = 0;
                        }
                        break;
                    case 0x200:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 0x18F) || func_8007531C(i, 0x18F, (s32)(ratio * -4.0f), (s32)(ratio * 86.0f), 2.0f * ratio, 2.0f * ratio) != inactive) { effect->fade46.v[0] = source->fade46.v[0]; effect->fade46.v[1] = source->fade46.v[1]; func_80074D88(effect, &D_8014CF00, 1); effect->frame_3F = effect->frame_3E; }
                        break;
                    case 0x400:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 0x190) || func_8007531C(i, 0x190, (s32)(ratio * -2.0f), (s32)(ratio * 85.0f), ratio * 2.7f, ratio * 2.7f) != inactive) { effect->fade46.v[0] = source->fade46.v[0]; effect->fade46.v[1] = source->fade46.v[1]; func_80074D88(effect, &D_8014CF08, 1); effect->frame_3F = effect->frame_3E; }
                        break;
                    case 0x100:
                        if ((effect->id_02 != inactive && effect->sprite_3C == 0x182) || func_8007531C(i, 0x182, (s32)(ratio * -4.0f), (s32)(ratio * 86.0f), 2.0f * ratio, 2.0f * ratio) != inactive) { effect->fade46.v[0] = source->fade46.v[0]; effect->fade46.v[1] = source->fade46.v[1]; func_80074D88(effect, &D_8014CEF0, 1); effect->frame_3F = effect->frame_3E; }
                        break;
                }
            }
        }
    }
}

/* ==== func_80075E5C ==== */
s32 func_80041FF8(void);
void func_80075E5C(s32 idx, s32 mode) {
    Unit *e = &D_801DEAB4[idx];
    s32 changed = 0;
    s32 flag = 0;
    if (e->id_02 == -1) return;
    if (e->action_09 == mode) return;
    if (!(mode >= 3 && mode < 8) || !(e->action_09 >= 3 && e->action_09 < 8)) changed = 1;
    if (mode >= 3 && mode < 8) {
        if (!(u8)func_80041FF8()) {
            if (!(e->flags_0A & 0x4000)) flag = 1;
        }
    }
    e->action_09 = mode;
    func_80074BF4(idx, changed, flag);
}

/* ==== func_80075F44 ==== */
s32 func_80075F44(s32 kind, s32 level, s32 sub) {
    Species *table = D_8013FA90;
    SpeciesRef *entry;
    if (sub >= 127 || kind < 2 || kind >= 215) return -1;
    level = level > 0 ? level - 1 : 0;
    switch (kind) {
    case 0x37:
        if (level >= 4) level = 3;
        entry = &D_8013FE90[level];
        break;
    case 0x31:
        if (level >= 3) level = 2;
        entry = &D_8013FEB0[level];
        break;
    case 0x25:
        if (level >= 3) level = 2;
        entry = &D_8013FEC8[level];
        break;
    default:
        entry = &D_8013F3D0[kind];
        break;
    }
    {
        /* local-arithmetic-qualification: in-bounds table[index] emits the base first;
           retain the byte offset as the first operand of this local address sum. */
        Species *byIndex = (Species *)((u32)entry->index * sizeof(Species) + (u32)table);
        Species *bySub = &D_8013FA90[sub];
        if (sub == 0) entry->species = byIndex;
        else entry->species = bySub;
    }
    return 0;
}

/* ==== func_80076044 ==== */
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
s32 func_80076044(s32 index, s32 resource, s32 mode, s32 value, s32 enabled) {
    SpeciesRef *entry;
    s32 changed = 0;
    s32 randomize = 0;
    Unit *self = &D_801DEAB4[index];
    if (self->id_02 == -1 || !self->buf_50 || !self->buf_58 || !self->buf_54) return -1;
    if (self->sprite_3C != resource) {
        entry = func_80074784(self->id_02, self->level_06);
        if (!resource) { changed = 1; resource = entry->species->anim_06; }
        self->sprite_3C = resource;
        /* local-arithmetic-qualification: ROM symbols and segmented pointers are in a different address space from RAM; in-bounds CPU pointer arithmetic cannot express the integer cartridge address required by DMA. */
        func_8006AAF0(self->buf_50, ((u32)D_5000000 & 0xFFFFFF) + (u32)D_00E53DB0 + resource * 16, 16);
        func_8006AAF0(self->buf_58, ((u32)self->buf_50->table_0C.segmented & 0xFFFFFF) + (u32)D_00E53DB0, self->buf_50->count_08 * 12);
        func_8006AAF0(self->buf_54, ((u32)self->buf_50->table_04.segmented & 0xFFFFFF) + (u32)D_00E53DB0, (self->buf_50->count_00 & 31) * 4);
        self->buf_50->table_0C.resident = self->buf_58;
        self->buf_50->table_04.resident = self->buf_54;
        self->desc_4C = self->buf_50;
        func_8007482C(index, entry->palette ? entry->palette : self->level_06);
    }
    if (self->active_07 != mode) changed = 1;
    if ((u32)(self->action_09 - 3) < 5 && !(func_80041FF8() & 0xFF)) {
        if (!(self->flags_0A & 0x4000)) randomize = 1;
    }
    func_80074BF4(index, changed, randomize);
    if (mode == 2) self->frame_3E = value;
    self->active_07 = mode;
    self->frameStep_40 = enabled;
    return 0;
}

/* ==== func_80076264 ==== */
extern s32 D_801E4E70;
/* All six parameters are optional f32 out-pointers (each store is guarded by bnel reg,zero). */
void func_80061AB8(f32 *arg0, f32 *arg1, f32 *minX, f32 *minZ, f32 *maxX, f32 *maxZ);
s32 func_8005B2CC(void);
void func_8005ADAC(s32 arg0, s32 arg1, s32 arg2);
s32 func_800627C4(void);
void func_80077498(s32 arg0);
void func_80076F48(s32 arg0);
s32 func_80084E20(void);
void func_8006E6D8(void);

void func_80076264(s32 mode) {
    s32 i;
    if (D_8013D8E4 != 0) {
        if (!(func_80041FF8() & 255) && D_8013D8E0 == 1) {
            D_8013D8E4 = 1;
            return;
        }
        for (i = 0; i < 30; i++) {
            Unit *self = &D_801DEAB4[i];
            SpeciesRef *entry;
            const Anim *animation;
            s32 ticks;
            if (self->id_02 == -1 || self->state_08 == 1 || self->active_07 != 1) continue;
            entry = func_80074784(self->id_02, self->level_06);
            switch (self->action_09) {
            case 0:
                animation = &D_8014D1B4[entry->species->anim_03];
                ticks = 0;
                break;
            case 2:
                ticks = 0;
                if (self->id_02 == 0x17) animation = &D_8014D1B4[15];
                else animation = &D_8014D1B4[entry->species->anim_03];
                break;
            case 1:
                if (self->id_02 == 0x17) { animation = &D_8014D1B4[14]; ticks = 1; }
                else { animation = &D_8014D1B4[13]; ticks = 0; }
                break;
            case 4:
                ticks = 1;
                animation = &D_8014D1B4[entry->species->anim_03];
                if (self->mode_05 == 0 && (self->flags_0A & 0x4000) && D_801A79E8[i].count <= 0 && self->frame_3E == 8) continue;
                break;
            case 3:
                ticks = mode == 2 ? 6 : 2;
                animation = &D_8014D1B4[entry->species->anim_03];
                break;
            case 6:
                ticks = 3;
                animation = &D_8014D1B4[entry->species->anim_03];
                if (self->mode_05 == 0 && (self->flags_0A & 0x4000) && D_801A79E8[i].count <= 0 && self->frame_3E == 8) continue;
                break;
            case 7:
                ticks = 6;
                animation = &D_8014D1B4[entry->species->anim_03];
                if (self->mode_05 == 0 && (self->flags_0A & 0x4000) && D_801A79E8[i].count <= 0 && self->frame_3E == 8) continue;
                break;
            case 5:
                ticks = 2;
                animation = &D_8014D1B4[entry->species->anim_03];
                if (self->mode_05 == 0 && (self->flags_0A & 0x4000) && D_801A79E8[i].count <= 0 && self->frame_3E == 8) continue;
                break;
            default:
                ticks = 2;
                animation = &D_8014D1B4[entry->species->anim_03];
                if (self->mode_05 == 0 && (self->flags_0A & 0x4000) && D_801A79E8[i].count <= 0 && self->frame_3E == 8) continue;
                break;
            }
            if (self->mode_05 == 1) {
                switch (self->id_02) {
                case 0x17:
                    animation = &D_8014D1B4[16];
                    ticks = 2;
                    if (animation->frames[self->step_42].id == 0 && (func_8002A9B0() & 63) == 0) {
                        self->timer_43 = (s8)(func_8002A9B0() % 127 + 30) % 127;
                    }
                    break;
                case 0x1B:
                    ticks = 2;
                    animation = &D_8014D1B4[18];
                    if (self->step_42 >= 3) func_800751B4(i, (self->flags_0A & 0x4000) | ticks);
                    break;
                }
            }
            func_80074D88(self, animation, ticks);
        }
    }
    D_8013D8E4 = 1;
}

void func_800765AC(void) {
    s32 i;
    for (i = 0; i < 30; i++) {
        D_801A79E8[i].count = 0;
        D_801A79E8[i].anim[3] = -1;
    }
}

s32 func_800765E8(s32 index, s32 x, s32 y, s32 z, s32 rx, s32 ry, s32 rz, s32 time)
{
    Zone *entry = &D_801A79E8[index];
    s32 slot;

    if (entry->count >= 0) {
        if (entry->count >= 3) {
            slot = -1;
            entry->count = 3;
        } else {
            slot = entry->count++;
        }
    } else {
        slot = -1;
    }

    if (slot == -1) {
        entry->to[2].x = rx;
        entry->to[2].y = ry;
        entry->to[2].state = rz;
        entry->anim[2] = time;
    } else {
        entry->from[slot].x = x;
        entry->from[slot].y = y;
        entry->from[slot].state = z;
        entry->to[slot].x = rx;
        entry->to[slot].y = ry;
        entry->to[slot].state = rz;
        entry->anim[slot] = time;
    }
    entry->anim[3] = time;
    return 1;
}

void func_800766A8(s32 index, s32 value) {
    if (D_801A79E8[index].count == 0) {
        if (D_801DEAB4[index].dir_4A != value) {
            D_801DEAB4[index].dir_4A = value;
        }
    } else {
        D_801A79E8[index].anim[3] = value;
    }
}

void func_80076714(s32 value) {
    if (D_8013D8E4 != value) {
        D_8013D8E4 = value;
    }
}

static inline s32 clampMin(s32 value, s32 limit) {
    return (value < limit) ? limit : value;
}

static inline s32 clampMax(s32 value, s32 limit) {
    return (value > limit) ? limit : value;
}

void func_80076734(s32 force, s32 arg1) {
    Vec3f pos;
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    s32 ok = 0;
    s32 n = 1;
    s32 idx;
    u32 off; /* byte offset of this zone; kept separate to match codegen */
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 cx;
    s32 cz;
    s32 i;
    s32 j;
    s32 k;
    s32 count;
    Zone *zone;
    Unit *actor;

    if (D_8013D8CC == 0) {
        return;
    }
    func_80061AB8(0, 0, &minX, &minZ, &maxX, &maxZ);
    D_8013D8E4 = 0;
    idx = func_80074114();
    off = idx * sizeof(Zone);
    func_800593F8(&pos);
    if (((Zone *)((u8 *)D_801A79E8 + off))->count != 0) {
        if ((u8)func_80041FF8() == 0 && func_8005B2CC() == 0) {
            func_8005ADAC(8, 1, 1);
        }
    } else {
        ok = 1;
        switch ((u32)func_800627C4()) {
        case 1:
            left = -10;
            right = 10;
            top = -10;
            bottom = 8;
            cx = (s32)pos.x >> 5;
            cz = (s32)pos.z >> 5;
            break;
        case 3:
            cx = D_801DEAB4[idx].x_0C >> 7;
            cz = D_801DEAB4[idx].z_10 >> 7;
            if (D_801E4E70 == 0 || D_8013D8F0 == 1 || D_8013D8FC == 1) {
                left = -5;
                right = 5;
                top = -5;
                bottom = 4;
            } else {
                left = (s32)((minX - 32.0f) * 0.03125f) - cx;
                right = (s32)((maxX + 32.0f) * 0.03125f) - cx;
                top = (s32)((minZ - 32.0f) * 0.03125f) - cz;
                bottom = (s32)((maxZ + 32.0f) * 0.03125f) - cz;
                left = clampMin(left, -5);
                top = clampMin(top, -5);
                right = clampMax(right, 5);
                bottom = clampMax(bottom, 4);
            }
            break;
        case 2:
        default:
            left = -5;
            right = 5;
            top = -5;
            bottom = 4;
            cx = D_801DEAB4[idx].x_0C >> 7;
            cz = D_801DEAB4[idx].z_10 >> 7;
            break;
        }
        for (i = 0; i < 30; i++) {
            zone = &D_801A79E8[i];
            actor = &D_801DEAB4[i];
            if (actor->id_02 == -1) {
                continue;
            }
            if (zone->count == 0) {
                continue;
            }
            if (D_801DEAB4[i].state_08 == 1) {
                continue;
            }
            count = (zone->count < 0) ? 1 : zone->count;
            for (j = 0; j < count; j++) {
                if ((zone->from[j].x >= cx + left && zone->from[j].x <= cx + right
                     && zone->from[j].y >= cz + top && zone->from[j].y <= cz + bottom)
                    || (zone->to[j].x >= cx + left && zone->to[j].x <= cx + right
                        && zone->to[j].y >= cz + top && zone->to[j].y <= cz + bottom)) {
                    ok = 0;
                    break;
                }
            }
            if (ok == 0) {
                break;
            }
        }
    }
    if ((u8)func_80041FF8() != 0) {
        ok = 0;
    }
    func_80077498(0);
    if (force != 0 || ok != 0) {
        D_8013D8E4 = force ? 1 : 2;
        func_80076F48(8);
    } else {
        n = 1;
        if (D_8013D8E8 == 1) {
            n = 2;
        }
        j = 1;
        while (1) {
            D_8013D8E4 = 1;
            for (k = 0; k < n; k++) {
                func_80076F48(j);
                j++;
            }
            if (arg1 != 0) {
                func_80084E20();
            }
            if (j >= 9) {
                break;
            }
            func_8006E6D8();
        }
    }
    func_80077498(1);
    func_8006E6D8();
    func_800765AC();
}
/* ==== func_80076C34 ==== */

extern void func_8004194C(s32, s32 *, s32 *);
extern s32 func_800625FC(s32, s32);
extern s32 func_80062554(s32, s32);
s32 func_80076C34(s32 arg0, s32 arg1, s32 arg2) {
    s32 x = 0, y = 0;
    s32 kind = 0;
    s32 result = -1;
    s32 height;
    Zone *layout;
    Unit *state;
    switch ((u32)func_800627C4()) {
    case 2: height = -4; break;
    case 1: height = -15; break;
    case 3:
    default: height = -8; break;
    }
    layout = &D_801A79E8[arg0];
    state = &D_801DEAB4[arg0];
    switch (arg1) {
    case 0:
        kind = layout->to[arg2].state;
        x = layout->to[arg2].x;
        y = layout->to[arg2].y;
        break;
    case 2:
        kind = func_80042734(arg0);
        func_8004194C(arg0, &x, &y);
        break;
    case 1:
        kind = layout->from[arg2].state;
        x = layout->from[arg2].x;
        y = layout->from[arg2].y;
        break;
    }
    switch (kind) {
    /* ODD_C: kind 0 (no point) leaves result at -1; the label shapes codegen: without it 52 words differ (564 vs 576 bytes). */
    case 0: break;
    case 1:
        if (state->id_02 == 0x49) {
            result = (height - (s32)(func_80074784(0x49, state->level_06)->species->height_04 * state->scale_20)) * 4;
        } else if (func_800625FC(x, y) & 0x80) result = -128;
        else result = func_80062554(x, y) * 4;
        break;
    case 2:
        if (func_800625FC(x, y) & 0x2080) result = 0;
        else result = func_80062554(x, y) * 4;
        break;
    }
    return result;
}

/* ==== func_80076E74 ==== */
s32 func_800625FC(s32 arg0, s32 arg1);
s32 func_80062554(s32 arg0, s32 arg1);

s32 func_80076E74(s32 arg0, s32 arg1) {
    if (func_800625FC(arg0, arg1) & 0x80) {
        return -0x20;
    }
    return func_80062554(arg0, arg1);
}

/* ==== func_80076EBC ==== */
s32 func_800625FC(s32 x, s32 y);
s32 func_80062554(s32 x, s32 y);
s32 func_80076EBC(s32 x, s32 y) {
    if (func_800625FC(x, y) & 0x2000) {
        switch ((u32)func_800627C4()) {
        case 2:
            return -4;
        case 1:
            return -15;
        case 3:
            return -8;
        default:
            return -8;
        }
    }
    return func_80062554(x, y);
}

/* ==== func_80076F48 ==== */
typedef union { s32 word; struct { u16 x, y; } p; } Pos;
void func_80061820(s32 x, s32 y);
void func_80061908(Pos x, Pos y);
void func_80061A20(f32 progress);
float func_80032360(float x);
s32 func_800429E0(s32 id);
s32 func_80042A08(s32 value);
s32 func_800625FC(s32 a, s32 b);

void func_80076F48(s32 step)
{
    s32 focus = func_80074114();
    s32 i;

    for (i = 0; i < 30; i++) {
        Zone *g = &D_801A79E8[i];
        Unit *u = &D_801DEAB4[i];
        s32 hit;
        s32 prev;
        s32 cur;
        s32 j;
        s32 n;
        s32 anim;

        hit = 0;
        if (u->id_02 == -1 || g->count == 0) {
            continue;
        }
        prev = 0;
        cur = 8;
        n = g->count;
        for (j = 0; j < n; j++) {
            cur = (s32)((f32)(j + 1) * 8.0f / (f32)n + 0.5);
            if (cur >= step) {
                break;
            }
            prev = cur;
        }
        if (step == prev + 1 && i == focus) {
            Pos px, py;
            func_80061820(g->from[j].x, g->from[j].y);
            px.word = g->to[j].x;
            py.word = g->to[j].y;
            func_80061908(px, py);
        }
        if (step == cur) {
            if (i == focus) {
                Pos px, py;
                func_80061820(g->from[j].x, g->from[j].y);
                px.word = g->to[j].x;
                py.word = g->to[j].y;
                func_80061908(px, py);
                func_80061A20(100.0f);
            }
            u->x_0C = (g->to[j].x << 7) + 0x40;
            u->z_10 = (g->to[j].y << 7) + 0x40;
            u->y_0E = func_80076C34(i, 0, j);
        } else {
            f32 t = (f32)(step - prev) / (f32)(cur - prev);
            s32 a, b;
            f32 z;

            u->x_0C = (s32)((f32)((g->to[j].x - g->from[j].x) << 7) * t + (f32)((g->from[j].x << 7) + 0x40));
            u->z_10 = (s32)((f32)((g->to[j].y - g->from[j].y) << 7) * t + (f32)((g->from[j].y << 7) + 0x40));
            if (i == focus) {
                func_80061A20(t * 100.0);
            }
            a = func_80076C34(i, 0, j);
            b = func_80076C34(i, 1, j);
            if (g->to[j].state != g->from[j].state) {
                z = func_80032360(t * 3.141592654) * 32.0f * 4.0f;
            } else {
                z = (f32)(a - b) * t + (f32)b;
            }
            u->y_0E = (s32)z;
        }
        if (!func_800429E0(i) && !func_80042A08(i)) {
            if ((func_800625FC(g->from[j].x, g->from[j].y) & 0x80)
                || (func_800625FC(g->to[j].x, g->to[j].y) & 0x80)) {
                func_80076044(i, u->sprite_3C, 2, 0, 1);
                hit = 1;
            }
        }
        if (step == cur) {
            if (step != 8) {
                if ((func_800625FC(g->from[j + 1].x, g->from[j + 1].y) & 0x80)
                    || (func_800625FC(g->to[j + 1].x, g->to[j + 1].y) & 0x80)) {
                    hit = 0;
                }
            }
            if (hit == 1) {
                s32 frame;
                s32 flags;

                switch (u->action_09) {
                case 2:
                    if (u->id_02 == 0x17) {
                        frame = 0x24;
                        flags = 1;
                    } else {
                        frame = 8;
                        flags = 3;
                    }
                    break;
                case 1:
                    if (u->id_02 == 0x17) {
                        frame = 0x22;
                        flags = 0;
                    } else {
                        frame = 8;
                        flags = 3;
                    }
                    break;
                case 0:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                default:
                    frame = 8;
                    flags = 3;
                    break;
                }
                func_80076044(i, u->sprite_3C, 1, frame, flags);
            }
        }
        if (step == 8) {
            anim = g->anim[3];
        } else {
            anim = g->anim[j];
        }
        if (anim != -1) {
            u->dir_4A = anim;
        }
    }
}

/* ==== func_80077498 ==== */
s32 func_8007BE1C(s32 x, s32 y);
void func_80052260(s16 id);
void func_80077498(s32 mode) {
    s32 changed = 0;
    s32 i;
    for (i = 0; i < 30; i++) {
        Zone *g = &D_801A79E8[i];
        Unit *u = &D_801DEAB4[i];
        s32 j;
        if (u->id_02 == -1 || g->count == 0 || u->fade46.v[1] == 0 || u->state_08 == 1) {
            continue;
        }
        for (j = 0; j < g->count; j++) {
            if (g->to[j].state == g->from[j].state) {
                continue;
            }
            if (mode == 0 && g->from[j].state == 1) {
                changed = 1;
                func_8007BE1C(g->from[j].x, g->from[j].y);
            }
            if (mode == 1 && g->to[j].state == 1) {
                changed = 1;
                func_8007BE1C(g->to[j].x, g->to[j].y);
            }
        }
    }
    if (changed) {
        func_80052260(0x20);
    }
}
/* ==== func_800775F0 ==== */

s32 func_80041574(s32 a, s32 b);
s32 func_800421A8(void);
s32 func_8005CB7C(void);

void func_800775F0(void) {
    Vec3f pos;
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    s32 mode;
    s32 cx;
    s32 cz;
    s32 self;
    s32 home;
    s32 i;
    Unit *u;
    f32 dx, dz, m;

    mode = func_800627C4();
    func_80061AB8(0, 0, &minX, &minZ, &maxX, &maxZ);
    func_800593F8(&pos);
    cx = (s32)pos.x >> 5;
    cz = (s32)pos.z >> 5;
    self = func_80074114();
    home = func_80041574(D_801DEAB4[self].x_0C >> 7, D_801DEAB4[self].z_10 >> 7);

    for (i = 0; i < 30; i++) {
        u = &D_801DEAB4[i];
        if (u->id_02 == -1 || u->state_08 != 0) {
            continue;
        }
        if (D_8013D8F0 == 1 || D_801E4E70 == 0 || D_8013D8FC == 1) {
            u->fade46.v[1] = 0xFF;
        } else {
            dx = u->x_0C * 0.25f;
            dz = u->z_10 * 0.25f;
            if (dx < minX - 64.0f || maxX + 64.0f < dx || dz < minZ - 64.0f || maxZ + 64.0f < dz) {
                u->fade46.v[1] = 0;
            } else {
                dx = (dx < minX) ? minX - dx : ((maxX < dx) ? dx - maxX : 0.0f);
                dz = (dz < minZ) ? minZ - dz : ((maxZ < dz) ? dz - maxZ : 0.0f);
                m = (__builtin_sqrtf(dx * dx + dz * dz) - 48.0f) * 15.9375f;
                if (m < 0.0f) {
                    u->fade46.v[1] = 0xFF;
                } else if (m >= 255.0f) {
                    u->fade46.v[1] = 0;
                } else {
                    u->fade46.v[1] = 255.0f - m;
                }
            }
        }
        if (mode == 1) {
            if (func_800421A8() != 0) {
                u->fade46.v[1] = 0xFF;
            } else if (home == func_80041574(u->x_0C >> 7, u->z_10 >> 7)) {
                u->fade46.v[1] = 0xFF;
            } else {
                u->fade46.v[1] = 0;
            }
        }
        if (func_8005CB7C() == 1 && self != i) {
            u->fade46.v[1] = 0;
        }
    }

    for (i = 0; i < 30; i++) {
        u = &D_801DEAB4[i];
        if (u->id_02 == -1 || u->state_08 == 1 || u->fade46.v[1] != 0xFF) {
            continue;
        }
        if (mode == 1 || mode == 4) {
            continue;
        }
        {
            s32 x = u->x_0C >> 2;
            s32 z;
            if (x < (cx - 5) * 32 + 16 || x > (cx + 5) * 32 + 16
                || (z = u->z_10 >> 2, z < (cz - 5) * 32 + 16) || z > (cz + 4) * 32 + 16) {
                dx = (f32)(u->x_0C >> 2) - pos.x;
                dz = (f32)(u->z_10 >> 2) - pos.z;
                /* distance beyond the screen band: x in [-160, 160], z in [-160, 128] */
                if (dx >= 0.0f) {
                    dx = dx - 160.0f;
                } else {
                    dx = -(dx - -160.0f);
                }
                if (dz >= 0.0f) {
                    dz = dz - 128.0f;
                } else {
                    dz = -(dz - -160.0f);
                }
                m = dz;
                if (m < dx) {
                    m = dx;
                }
                if (m >= 48.0f) {
                    m = 256.0f;
                } else {
                    m = m * (256.0 / 48.0);
                }
                if (m < 255.0f) {
                    u->fade46.v[1] = 255 - (s32)m;
                } else {
                    u->fade46.v[1] = 0;
                }
            } else {
                u->fade46.v[1] = 0xFF;
            }
        }
    }
}

/* ==== func_80077BA4 ==== */
s32 func_80077BA4(s32 kind, s32 value) {
    s32 index;

    switch (kind) {
    case 0x17:
        index = 0;
        break;
    case 0x1A:
        index = 1;
        break;
    default:
        return -1;
    }
    D_8013D904[index].state_06 = value;
    return 0;
}

/* ==== func_80077BE4 ==== */
void func_80077BE4(s32 kind, s32 slot, s32 value) {
    s32 index;
    switch (kind) {
    case 0x17: index = 0; break;
    case 0x1A: index = 1; break;
    default: return;
    }
    if (slot == 3) D_8013D904[index].weaponFlag_08 = value;
    else if (slot == 4) D_8013D904[index].shieldFlag_0A = value;
}

/* ==== func_80077C4C ==== */
/* Equipment descriptor (4 bytes): +0 model/tile id (lhu), +2 percentage
 * byte (lbu), +3 never accessed. */
typedef struct { u16 id; u8 percent; u8 unk3; } EquipInfo;
/* Equipment descriptor tables indexed by kind minus the first kind of each
 * class. They are separate symbols in the original: the index bias is computed
 * at run time (addiu -0x32/-0xAA/-0x59 before the shift) instead of being
 * folded into a base address, which GCC does for an offset into one array. */
extern EquipInfo D_8013F24C[];
extern EquipInfo D_8013F2E8[];
extern EquipInfo D_8013F2EC[];

s32 func_80077C4C(s32 who, s32 slot, s32 id)
{
    s32 extra = 0;
    f32 scale = 1.0f;
    f32 ratio = 1.0f;
    s32 member;
    s32 party; /* equipment row; each row owns two model slots */
    s32 index;
    s32 kind;
    Equip *equip;
    Unit *leader;
    Unit *m;

    member = func_80041C64(who);
    if (member == -1) {
        return -1;
    }
    leader = D_801DEAB4;
    if (leader[member].id_02 == -1) {
        return -1;
    }
    switch (who) {
    case 0x17:
        party = 0;
        break;
    case 0x1A:
        party = 1;
        {
            f32 own = func_80074784(0x17, leader->level_06)->species->size_02 / 100.0f;
            ratio = own / (func_80074784(0x1A, D_801DEAB4[member].level_06)->species->size_02 / 100.0f);
        }
        break;
    default:
        return -1;
    }
    equip = &D_8013D904[party];
    switch (slot) {
    case 3:
        index = party * 2;
        equip->weapon_00 = id;
        equip->weaponFlag_08 = 0;
        kind = equip->weapon_00;
        if (kind != -1) {
            extra = D_8013F24C[kind - 0x32].id;
            scale = D_8013F24C[kind - 0x32].percent / 100.0f;
            scale *= ratio;
        }
        if (kind == 0x3A || kind == 0x53 || kind == 0x54 || kind == 0x55) {
            equip->weaponFlag_08 = 1;
        }
        break;
    case 9:
        index = party * 2;
        equip->weapon_00 = id;
        equip->weaponFlag_08 = 0;
        kind = equip->weapon_00;
        if (kind != -1) {
            extra = D_8013F2E8[kind - 0xAA].id;
            scale = D_8013F2E8[kind - 0xAA].percent / 100.0f;
            scale *= ratio;
        }
        break;
    case 4:
        index = party * 2 + 1;
        equip->shield_04 = id;
        equip->shieldFlag_0A = 0;
        kind = equip->shield_04;
        if (kind != -1) {
            extra = D_8013F2EC[kind - 0x59].id;
            scale = D_8013F2EC[kind - 0x59].percent / 100.0f;
            scale *= ratio;
        }
        if (kind == 0x59 || kind == 0x61 || kind == 0x5E) {
            equip->shieldFlag_0A = 1;
        }
        break;
    default:
        return -1;
    }
    if (id < 0) {
        func_80074778(&D_801D8FFC[index]);
        return index;
    }
    index = func_80074500(D_801D8FFC, index, 0, id, extra);
    if (index != -1) {
        m = &D_801D8FFC[index];
        m->type_00 = 1;
        m->attach_37 = 1;
        m->side_12 = 0;
        m->slot_13 = member;
        m->scale_1C = scale;
        m->scale_20 = scale;
        m->scale_24 = scale;
        m->show_44 = 2;
        m->slot_45 = index;
        m->active_07 = 1;
        equip->state_06 = 0;
    }
    return index;
}

/* ==== func_80077FE4 ==== */
void func_80079560(s32 a, s32 index, s32 flag);

/* Refreshes the two equipment display units (weapon at 2*i, shield at 2*i+1)
 * of both party members from their owners' facing and equipment state.
 * Each slot's unit is checked through its own pointer and then handed to the
 * shared `unit` pointer that the refresh code works on. */
void func_80077FE4(void) {
    s32 i;
    s32 j;
    s32 id;
    Unit *src;
    Unit *weapon;
    Unit *shield;
    Unit *unit;
    const u8 *entry;
    u32 index;

    for (i = 0; i < 2; i++) {
        switch (i) {
        case 0:
            id = 0;
            break;
        case 1:
            id = func_80041C64(0x1A);
            break;
        default:
            continue;
        }
        if (id == -1) {
            for (j = 0; j < 2; j++) {
                func_80074778(&D_801D8FFC[j + 2 * i]);
                D_8013D904[i].weapon_00 = -1;
                D_8013D904[i].shield_04 = -1;
                D_8013D904[i].state_06 = -1;
            }
            continue;
        }
        src = &D_801DEAB4[id];
        index = src->frame_3F;
        if (D_8013D904[i].state_06 == -1 || index >= 0x20
            || (src->id_02 != 0x17 && src->id_02 != 0x1A)
            || (src->sprite_3C != 0xA9 && src->sprite_3C != 0x90)) {
            func_80079560(1, 2 * i, 1);
            func_80079560(1, 2 * i + 1, 1);
            continue;
        }
        if (D_8013D904[i].shield_04 != -1) {
            shield = &D_801D8FFC[2 * i + 1];
            if (shield->id_02 != -1) {
                unit = shield;
                func_80079560(1, 2 * i + 1, 0);
                if (D_8013D904[i].shield_04 == 0x6D) {
                    entry = &D_8014CD7C[index];
                    unit->frame_3F = unit->frame_3E = *entry & 0xF;
                    unit->bob_34 = (*entry & 0x20) ? 1 : -1;
                    unit->x_0C = (s8)(src->id_02 == 0x1A ? D_8014CD10[index] : D_8014CCC8[index]);
                    unit->y_0E = (s8)(src->id_02 == 0x1A ? D_8014CD34[index] : D_8014CCEC[index]);
                } else {
                    entry = &D_8014CD58[index];
                    unit->frame_3F = unit->frame_3E = *entry & 0xF;
                    unit->bob_34 = (*entry & 0x20) ? 2 : -1;
                    unit->x_0C = (s8)(src->id_02 == 0x1A ? D_8014CDE8[index] : D_8014CDA0[index]);
                    unit->y_0E = (s8)(src->id_02 == 0x1A ? D_8014CE0C[index] : D_8014CDC4[index]);
                }
                if (D_8013D904[i].shieldFlag_0A == 1) {
                    switch (D_8013D904[i].shield_04) {
                    case 0x59:
                    case 0x5E:
                    case 0x61:
                        func_80074E70(unit, &D_8014CF64, 1);
                        break;
                    default:
                        unit->palette_41 = 0;
                        unit->timer_43 = 0;
                        unit->step_42 = 0;
                        break;
                    }
                } else {
                    unit->palette_41 = 0;
                    unit->timer_43 = 0;
                    unit->step_42 = 0;
                }
                unit->fade46.v[0] = src->fade46.v[0];
                unit->fade46.v[1] = src->fade46.v[1];
            }
        }
        if (D_8013D904[i].weapon_00 != -1) {
            weapon = &D_801D8FFC[2 * i];
            if (weapon->id_02 != -1) {
                unit = weapon;
                func_80079560(1, 2 * i, 0);
                if ((u16)(D_8013D904[i].weapon_00 - 0x4C) < 2) {
                    entry = &D_8014CCA4[index];
                } else {
                    entry = &D_8014CC80[index];
                }
                unit->frame_3F = unit->frame_3E = *entry & 0xF;
                unit->bob_34 = (*entry & 0x20) ? 1 : -1;
                unit->x_0C = (s8)(src->id_02 == 0x1A ? D_8014CD10[index] : D_8014CCC8[index]);
                unit->y_0E = (s8)(src->id_02 == 0x1A ? D_8014CD34[index] : D_8014CCEC[index]);
                if (D_8013D904[i].weaponFlag_08 == 1) {
                    switch (D_8013D904[i].weapon_00) {
                    case 0x4A:
                        func_80074E70(unit, &D_8014CF64, 1);
                        break;
                    case 0x3A:
                    case 0x53:
                    case 0x54:
                    case 0x55:
                        func_80074E70(unit, &D_8014CF6C, 1);
                        break;
                    default:
                        unit->palette_41 = 0;
                        unit->timer_43 = 0;
                        unit->step_42 = 0;
                        break;
                    }
                } else {
                    unit->palette_41 = 0;
                    unit->timer_43 = 0;
                    unit->step_42 = 0;
                }
                unit->fade46.v[0] = src->fade46.v[0];
                unit->fade46.v[1] = src->fade46.v[1];
            }
        }
    }
}

/* ==== func_80078590 ==== */
/* 4-byte unit descriptor: resource id and scale in percent. */
typedef struct { u16 resource; u8 scale, unused; } TileDesc;
/* One descriptor bank covers every index selected below (the original
 * addresses +0x1D0, +0x224 and +0x228 are elements 0x74.., 0x89 and 0x8A). */
extern TileDesc D_8013F020[];
/* Interface note: both callers here (0x80078740, 0x800787E0) and the three in
 * func_80078908 use the result without narrowing (daddu v1,v0); with a u8
 * return GCC inserts andi 0xFF. The callee's lbu already yields the full-width
 * value, so the result is s32 (canonical u8 to be corrected; see results.json). */
extern s32 func_800649C0(s32 row, s32 col);
s32 func_80078590(s32 handle, s32 kind, s32 variant, s32 row, s32 column)
{
    TileDesc *descriptor;
    Unit *actor;
    s32 terrain;
    u8 blue, green, red;
    switch (kind) {
    case 16:
        descriptor = &D_8013F020[variant];
        break;
    case 20:
        switch (variant) {
        case 0xF5: descriptor = &D_8013F020[0x89]; break;
        case 0xF6: descriptor = &D_8013F020[0x8A]; break;
        default: return -1;
        }
        break;
    case 15:
    case 19:
        return -1;
    default:
        descriptor = &D_8013F020[0x74 + kind];
        break;
    }
    actor = D_801E02A8;
    handle = func_80074500(actor, handle, 0, variant, descriptor->resource);
    if (handle == -1) return -1;
    actor += handle;
    actor->type_00 = 3;
    actor->scale_1C = actor->scale_20 = actor->scale_24 = descriptor->scale / 100.0f;
    switch (kind) {
    case 16:
        if (variant == 0xE7) actor->billboard_36 = 0;
        else actor->billboard_36 = 2;
        actor->raised_35 = 0;
        actor->speed_38 = 0.0f;
        actor->rot_28 = 4.712389f;
        if (D_8013D8F8 == 1) {
            terrain = func_800649C0(row, column);
            switch (terrain) {
            case 1:
            default:
                red = 255; green = red; blue = red;
                break;
            case 2:
                red = 255; green = 100; blue = 105;
                break;
            case 4:
                red = 180; green = 255; blue = red;
                break;
            case 0x81:
                red = 128; green = red; blue = red;
                break;
            case 0x82:
                red = 128; green = 50; blue = 52;
                break;
            case 0x84:
                red = 90; green = 128; blue = red;
                break;
            }
        } else {
            terrain = func_800649C0(row, column);
            switch (terrain) {
            case 1:
            case 2:
            case 4:
            default:
                red = 255; green = red; blue = red;
                break;
            case 0x81:
            case 0x82:
            case 0x84:
                red = 128; green = red; blue = red;
                break;
            }
        }
        actor->model.env18.rgba[0] = red;
        actor->model.env18.rgba[1] = green;
        actor->model.env18.rgba[2] = blue;
        break;
    case 20:
        switch (variant) {
        case 0xF5:
            actor->raised_35 = 1;
            actor->billboard_36 = 3;
            actor->speed_38 = 1.0f;
            break;
        case 0xF6:
            actor->speed_38 = 0.0f;
            actor->raised_35 = 0;
            actor->billboard_36 = 2;
            actor->rot_28 = 5.8904862f;
            break;
        }
        actor->step_42 = (func_8002A9B0() * handle) & 3;
        break;
    default:
        actor->raised_35 = 1;
        actor->billboard_36 = 3;
        actor->speed_38 = 0.75f;
        break;
    }
    actor->active_07 = 1;
    actor->show_44 = 255;
    actor->slot_45 = 0;
    return handle;
}
/* ==== func_80078908 ==== */

s32 func_8006262C(s32 a, s32 b);
/* Interface note: the caller passes the full s32 result of func_8006262C
 * (daddu a0,s1 at 0x80078A6C, no andi); with the canonical u8 parameter GCC
 * narrows at the call. The callee narrows internally (andi a0 before
 * func_800AC1AC), so the parameter is s32. */
s32 func_80041DBC(s32 arg0);
/* Interface note: all five results here are used unnarrowed (beql v0 at
 * 0x80078DBC, daddu v1,v0 at 0x80078DD4/0x8007909C/0x8007912C); a u8 return
 * makes GCC insert andi 0xFF. The callee's lbu yields the full value. */
s32 func_80064990(s32 row, s32 col);

/* Refresh the tile-object actors in the 11x10 window around the camera:
 * hide out-of-map/empty cells, (re)bind each cell's actor via func_80078590,
 * place it, fade it near the window edge and apply per-kind animation. */
void func_80078908(void)
{
    Vec3f cam;
    s32 camX, camZ;
    s32 x, z;
    s32 slot;
    Unit *actor;
    s32 id, kind, attr, flags;
    f32 dx, dz, d;
    s32 red, green, blue;
    s32 v, t;
    s32 r, g, b;
    s32 visible;

    func_800593F8(&cam);
    camX = (s32)cam.x >> 5;
    camZ = (s32)cam.z >> 5;
    for (x = camX - 5; x <= camX + 5; x++) {
        for (z = camZ - 5; z <= camZ + 4; z++) {
            slot = (z % 10) * 11 + x % 11;
            if ((u32)slot >= 110) continue;
            actor = &D_801E02A8[slot];
            if (func_8005CB7C() != 1 && (x >= 10 && x < 66 && z >= 10 && z < 44)) {
                id = func_8006262C(x, z);
                if (id == 0) { func_80074778(actor); continue; }
                kind = func_80041DBC(id);
                attr = func_800625FC(x, z);
                flags = attr & 0x31700000;
                if (flags == 0 && D_801E4E70 != 0) { func_80074778(actor); continue; }
                if (kind == 15) { func_80074778(actor); continue; }
                if (kind == 19) { func_80074778(actor); continue; }
                /* kind 16 cells without the 0x31700000 flags are shown only
                 * when the 0x08000000 attribute is set (and D_801E4E70 == 0). */
                visible = 0;
                if (kind == 16 && flags == 0) {
                    visible = D_801E4E70 == 0 && (attr & 0x08000000);
                    if (!visible) { func_80074778(actor); continue; }
                }
                if (actor->id_02 == -1 || actor->id_02 != id || actor->x_0C >> 7 != x || actor->z_10 >> 7 != z) {
                    slot = func_80078590(slot, kind, id, x, z);
                    if (slot == -1) continue;
                    actor = &D_801E02A8[slot];
                }
                actor->x_0C = (x << 7) + 0x40;
                actor->z_10 = (z << 7) + 0x40;
                if (kind == 16) actor->y_0E = (func_80062554(x, z) << 2) + 4;
                else actor->y_0E = func_80062554(x, z) << 2;
                if (x < camX - 4 || x > camX + 4 || z < camZ - 4 || z > camZ + 3) {
                    dx = (f32)((x << 5) + 16) - cam.x;
                    dz = (f32)((z << 5) + 16) - cam.z;
                    if (dx >= 0.0f) dx -= 128.0f;
                    else dx = -(dx - -128.0f);
                    if (dz >= 0.0f) dz -= 96.0f;
                    else dz = -(dz - -128.0f);
                    d = dz;
                    if (d < dx) d = dx;
                    if (d >= 48.0f) d = 256.0f;
                    else d *= 256.0 / 48.0;
                    if (d < 255.0f) actor->fade46.v[1] = 255 - (s32)d;
                    else actor->fade46.v[1] = 0;
                } else {
                    actor->fade46.v[1] = 255;
                }
            } else {
                func_80074778(actor);
                continue;
            }
            switch (kind) {
            case 16:
                switch (id) {
                case 0xD5:
                    if (func_80064990(x, z) != 0) actor->frame_3E = 1;
                    else actor->frame_3E = 0;
                    break;
                case 0xE7:
                    switch (func_80064990(x, z)) {
                    case 0:
                    default: actor->frame_3E = 0; break;
                    case 2: actor->frame_3E = 1; break;
                    case 4: actor->frame_3E = 2; break;
                    case 6: actor->frame_3E = 3; break;
                    }
                    func_80074E70(actor, &D_8014CF3C, 1);
                    break;
                case 0xE6:
                    func_80074E70(actor, &D_8014CF34, 1);
                    break;
                }
                if (D_8013D8F8 == 1) {
                    switch (func_800649C0(x, z)) {
                    case 1:
                    default:
                        red = 255; green = red; blue = red;
                        break;
                    case 2:
                        red = 255; green = 100; blue = 105;
                        break;
                    case 4:
                        red = 180; green = 255; blue = red;
                        break;
                    case 0x81:
                        red = 128; green = red; blue = red;
                        break;
                    case 0x82:
                        red = 128; green = 50; blue = 52;
                        break;
                    case 0x84:
                        red = 90; green = 128; blue = red;
                        break;
                    }
                } else {
                    switch (func_800649C0(x, z)) {
                    case 1:
                    case 2:
                    case 4:
                    default:
                        red = 255; green = red; blue = red;
                        break;
                    case 0x81:
                    case 0x82:
                    case 0x84:
                        red = 128; green = red; blue = red;
                        break;
                    }
                }
                if (func_800649C0(x, z) & 0x80) {
                    r = actor->model.env18.rgba[0];
                    g = actor->model.env18.rgba[1];
                    b = actor->model.env18.rgba[2];
                    if (r >= red) {
                        /* fade a quarter step toward the target, never below it */
                        t = r - red / 4;
                        v = (red < t) ? t : red;
                        actor->model.env18.rgba[0] = v;
                    }
                    if (g >= green) {
                        t = g - green / 4;
                        v = (green < t) ? t : green;
                        actor->model.env18.rgba[1] = v;
                    }
                    if (b >= blue) {
                        t = b - blue / 4;
                        v = (blue < t) ? t : blue;
                        actor->model.env18.rgba[2] = v;
                    }
                } else {
                    actor->model.env18.rgba[0] = red;
                    actor->model.env18.rgba[1] = green;
                    actor->model.env18.rgba[2] = blue;
                }
                break;
            case 2:
                if (func_80064990(x, z) != 0) {
                    actor->frame_3E = 1;
                    actor->raised_35 = 0;
                    actor->billboard_36 = 0;
                    actor->speed_38 = 0.0f;
                    actor->rot_28 = 4.712389f;
                    actor->y_0E = (func_80062554(x, z) << 2) + 4;
                } else {
                    actor->frame_3E = 0;
                    actor->raised_35 = 1;
                    actor->rot_28 = 0.0f;
                    actor->billboard_36 = 3;
                    actor->speed_38 = 0.75f;
                }
                break;
            case 20:
                switch (id) {
                case 0xF5:
                    switch (func_80064990(x, z)) {
                    case 1: actor->scale_1C = 0.45f; actor->scale_20 = 0.56f; break;
                    case 2: actor->scale_1C = 0.8f; actor->scale_20 = 1.0f; break;
                    case 4:
                    default: actor->scale_1C = 1.2f; actor->scale_20 = 1.8f; break;
                    }
                    func_80074D88(actor, &D_8014CF9C, 1);
                    break;
                case 0xF6:
                    switch (func_80064990(x, z)) {
                    case 1: actor->scale_1C = actor->scale_20 = 1.4f; break;
                    case 2: actor->scale_1C = actor->scale_20 = 2.45f; break;
                    case 4:
                    default: actor->scale_1C = actor->scale_20 = 3.5f; break;
                    }
                    func_80074D88(actor, &D_8014CFA4, 1);
                    break;
                }
                break;
            default:
                actor->palette_41 = 0;
                actor->timer_43 = 0;
                actor->step_42 = 0;
                break;
            }
            actor->frame_3F = actor->frame_3E;
        }
    }
}

/* ==== func_8007920C ==== */
typedef struct { unsigned short id; unsigned short pad; } Code8007920C;
extern Code8007920C D_8013F1F0[];
extern Code8007920C D_8013F360[];
s32 func_8007920C(s32 kind, s32 id, s32 x, s32 y) {
    s32 code;
    s32 index;
    Unit *slot;
    switch (kind) {
    case 0:
        code = id;
        break;
    case 0x10:
        code = D_8013F360[id - 0xD0].id;
        break;
    default:
        code = D_8013F1F0[kind].id;
        break;
    }
    index = func_80074500(D_801DD378, 0, 0x20, id, code);
    if (index == -1) return -1;
    slot = &D_801DD378[index];
    if (D_801D25A0[code] == 0) slot->show_44 = 0xFE;
    else slot->show_44 = 0xFF;
    slot->active_07 = 1;
    slot->type_00 = 4;
    slot->x_0C = (x << 7) + 0x40;
    slot->z_10 = (y << 7) + 0x40;
    slot->y_0E = 0;
    slot->raised_35 = 1;
    slot->slot_45 = 0;
    slot->frame_3E = 0;
    slot->frameStep_40 = 0;
    slot->scale_24 = 1.0f;
    slot->scale_20 = 1.0f;
    slot->scale_1C = 1.0f;
    slot->speed_38 = 0.75f;
    return index;
}

/* ==== func_8007935C ==== */
void func_8007935C(s32 i) {
    func_80074778(&D_801DD378[i]);
}

/* ==== func_80079394 ==== */
void func_80079394(void) {
    Unit *entry;
    Unit *other;
    Unit *last;

    entry = D_801DD378;
    last = entry + 31;
    for (; entry <= last; entry++) {
        if (entry->id_02 != -1 && entry->attach_37 == 1) {
            other = func_8007946C(entry->side_12, entry->slot_13);
            if (other != 0 && other->id_02 != -1) {
                entry->fade46.v[0] = other->fade46.v[0];
                entry->fade46.v[1] = other->fade46.v[1];
            }
        }
    }
}

/* ==== func_80079448 ==== */
Zone *func_80079448(s32 index)
{
    return &D_801A79E8[index];
}

/* ==== func_8007946C ==== */
Unit *func_8007946C(s32 side, s32 slot) {
    Unit *unit = 0;
    switch (side) {
    case 0:
        if ((u32)slot < 30) unit = &D_801DEAB4[slot];
        break;
    case 1:
        if ((u32)slot < 4) unit = &D_801D8FFC[slot];
        break;
    case 2:
        if ((u32)slot < 30) unit = &D_801D2C2C[slot];
        break;
    case 3:
        if ((u32)slot < 110) unit = &D_801E02A8[slot];
        break;
    case 4:
        if ((u32)slot < 32) unit = &D_801DD378[slot];
        break;
    }
    return unit;
}

/* ==== func_80079560 ==== */
void func_80079560(s32 a, s32 b, s32 mode){
    Unit *p = func_8007946C(a, b);
    if (p == 0) return;
    if (p->id_02 == -1) return;
    if (p->state_08 == mode) return;
    if (mode == 1) {
        if (p->flags_0A & 0x10) p->timer_18 = 30;
    }
    p->state_08 = mode;
}

/* ==== func_800795C8 ==== */
s32 func_80059830(s32 id);
void func_800795C8(void) {
    s32 k;
    for (k = 0; k < 2; k++) {
        Unit *list;
        Unit *u;
        s32 count;
        s32 i;
        if (k == 0) {
            list = D_801DEAB4;
            count = 30;
        } else {
            list = D_801DD378;
            count = 32;
        }
        for (i = 0; i < count; i++) {
            u = &list[i];
            if (u->id_02 == -1) {
                continue;
            }
            if (u->frameStep_40) {
                u->frame_3F = u->frame_3E + func_80059830(u->dir_4A) * u->frameStep_40;
            } else {
                u->frame_3F = u->frame_3E;
            }
        }
    }
}

/* ==== func_80079698 ==== */
void func_80079698(f32 m[4][4], f32 x, f32 y, f32 z) {
    if (x != 0.0f) {
        m[0][0] += m[0][3] * x;
        m[1][0] += m[1][3] * x;
        m[2][0] += m[2][3] * x;
        m[3][0] += m[3][3] * x;
    }
    if (y != 0.0f) {
        m[0][1] += m[0][3] * y;
        m[1][1] += m[1][3] * y;
        m[2][1] += m[2][3] * y;
        m[3][1] += m[3][3] * y;
    }
    if (z != 0.0f) {
        m[0][2] += m[0][3] * z;
        m[1][2] += m[1][3] * z;
        m[2][2] += m[2][3] * z;
        m[3][2] += m[3][3] * z;
    }
}

/* ==== func_80079800 ==== */
/* Scales the first three rows of a 4x4 float matrix by x, y and z, skipping
 * rows whose factor is exactly 1.0 (compared in double precision). */
void func_80079800(f32 m[4][4], f32 x, f32 y, f32 z) {
    if (x != 1.0) {
        m[0][0] *= x;
        m[0][1] *= x;
        m[0][2] *= x;
        m[0][3] *= x;
    }
    if (y != 1.0) {
        m[1][0] *= y;
        m[1][1] *= y;
        m[1][2] *= y;
        m[1][3] *= y;
    }
    if (z != 1.0) {
        m[2][0] *= z;
        m[2][1] *= z;
        m[2][2] *= z;
        m[2][3] *= z;
    }
}

/* ==== func_8007991C ==== */
extern float func_80032360(float);
extern float func_80027C60(float);
void func_8007991C(float matrix[4][4], float x, float y, float z) {
    float sx = func_80032360(x);
    float cx = func_80027C60(x);
    float sy = func_80032360(y);
    float cy = func_80027C60(y);
    float sz = func_80032360(z);
    float cz = func_80027C60(z);
    matrix[0][0] = cy * cz;
    matrix[0][1] = cy * sz;
    matrix[0][2] = -sy;
    matrix[0][3] = 0;
    matrix[1][0] = sx * sy * cz - cx * sz;
    matrix[1][1] = sx * sy * sz + cx * cz;
    matrix[1][2] = sx * cy;
    matrix[1][3] = 0;
    matrix[2][0] = cx * cy * cz + sx * sz;
    matrix[2][1] = cx * cy * sz - sx * cz;
    matrix[2][2] = cx * cy;
    matrix[2][3] = 0;
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1.0f;
}

/* ==== func_80079A80 ==== */
/* Whole three-float camera vector. */
typedef struct { f32 x, y, z; } Triple_800597E8;

/* 64-byte view block copied by func_80059B18 (read here as a float matrix). */
typedef struct { s32 w[16]; } Block40;

void func_800597E8(Triple_800597E8 *dst);
void func_80059B58(Mtx **out0, Mtx **out1, Mtx **out2);
void func_80030E70(f32 mf[4][4], f32 a, f32 x, f32 y, f32 z);
void func_8002CBC0(f32 mf[4][4], f32 nf[4][4], f32 res[4][4]);
s32 func_80042734(s32 cell);
f32 func_80032360(f32 x);
f32 func_80027C60(f32 x);
void func_8002CE20(f32 mf[4][4]);
/* Overlay 0x801E660C (overlay_136dc0) returns a sign-loaded halfword. */
s16 func_801E660C(s32 slot);
void func_80059B18(Block40 *dst);
Mtx *func_80070660(u32 count);

#define PI 3.141592654
#define TWO_PI 6.283185307179586

void func_80079A80(void) {
    Triple_800597E8 cam;
    f32 mf[4][4];
    f32 tmp[4][4];
    union {
        Block40 raw;
        f32 m[4][4];
    } view;
    f32 m0[4][4];
    f32 m1[4][4];
    f32 m2[4][4];
    f32 r1[4][4];
    f32 r2[4][4];
    Mtx *p0;
    Mtx *p1;
    Mtx *p2;
    Unit **out = D_801A76B0;
    Unit *tbl;
    Unit *u;
    Unit *parent;
    Mtx *mtx;
    SpeciesRef *info;
    s32 count;
    s32 side;
    s32 j;
    s32 offset;
    s32 time;
    s32 x;
    s32 y;
    s32 z;
    f32 pitch;
    f32 height;
    f32 a;
    f32 b;
    f32 c;
    f32 d;

    func_800597E8(&cam);
    func_80059B58(&p0, &p1, &p2);
    func_8002CE80(m0, p0);
    func_8002CE80(m1, p1);
    func_8002CE80(m2, p2);

    if (cam.x >= PI && cam.x < 2 * PI) {
        pitch = (cam.x <= 5.572388) ? 5.572388 : cam.x;
    } else {
        pitch = (cam.x >= 2 * PI - 5.572388) ? 2 * PI - 5.572388 : cam.x;
    }
    func_80030E70(r1, pitch * (180.0 / PI), 1.0f, 0.0f, 0.0f);
    func_8002CBC0(r1, m1, r1);

    if (cam.x >= PI && cam.x < 2 * PI) {
        pitch = (cam.x <= 5.722388) ? 5.722388 : cam.x;
    } else {
        pitch = (cam.x >= 2 * PI - 5.722388) ? 2 * PI - 5.722388 : cam.x;
    }
    func_80030E70(r2, pitch * (180.0 / PI), 1.0f, 0.0f, 0.0f);
    func_8002CBC0(r2, m1, r2);

    for (side = 0; side < 5; side++) {
        switch (side) {
        case 0:
            tbl = D_801DEAB4;
            count = 30;
            break;
        case 1:
            tbl = D_801D8FFC;
            count = 4;
            break;
        case 2:
            tbl = D_801D2C2C;
            count = 30;
            break;
        case 3:
            tbl = D_801E02A8;
            count = 110;
            break;
        case 4:
        default:
            tbl = D_801DD378;
            count = 32;
            break;
        }
        for (j = 0; j < count; j++) {
            tbl[j].flags_49 = 0;
        }
    }

    for (side = 0; side < 5; side++) {
        switch (side) {
        case 0:
            tbl = D_801DEAB4;
            count = 30;
            break;
        case 1:
            tbl = D_801D8FFC;
            count = 4;
            break;
        case 2:
            tbl = D_801D2C2C;
            count = 30;
            break;
        case 3:
            tbl = D_801E02A8;
            count = 110;
            break;
        case 4:
            tbl = D_801DD378;
            count = 32;
            break;
        default:
            continue;
        }
        for (j = 0; j < count; j++) {
            u = &tbl[j];
            /* FAKEMATCH: dead store. Every path reassigns x before its use, so
             * flow (after loop) deletes it; no instruction of it survives.
             * Mechanism (gcc 2.8.1 loop.c): its 0.25 load is the first member
             * of the 0.25 movable group (combine_movables), so the inner pass
             * hoists 0.25 before the wobble constants. In the outer pass every
             * already-moved register doubles insn_count (move_movables,
             * loop.c:1618-1642: threshold*savings*lifetime >= insn_count with
             * threshold 61 - 3 per move); with 0.25 ahead of -1, 1, 1/256 and
             * 2pi, 0.1 becomes the sixth candidate and fails, so 0.1/0.9/0.85
             * and &mf stay in the inner preheader as in the ROM, and 0.25 is
             * outer-hoisted (refs 13 over the outer loop), dropping below c
             * in global.c allocno_compare so c gets fs5 and 0.25 is reloaded
             * per use (the three li.d 0.25 sites at 8007A364/A3BC/A3F8).
             * Measured alternatives (differing words, 3072-byte slot):
             *   no dead store                         602 (0.1 to outer, 0.25 in fs5)
             *   function-scope double quarter = 0.25  71 (0.1 still outer-hoisted)
             *   0.1/0.9/0.85 as loop-top locals       601 (&mf outer-hoisted)
             *   x, z and y computed early and used    761
             *   same dead store in the init block     15 (after the 1.0 inits)
             *   dead store of z or of y instead       0 (same mechanism)
             *   dead store after u->model.matrix2C = 0        0 (same mechanism) */
            x = u->x_0C * 0.25;
            if (u->id_02 == -1) {
                continue;
            }
            u->timer_18++;
            if (u->state_08 == 1) {
                continue;
            }
            if (u->fade46.v[1] == 0) {
                continue;
            }
            if (u->fade46.v[0] == 0) {
                continue;
            }
            u->model.matrix2C = 0;
            switch (u->attach_37) {
            /* ODD_C: attachment zero has its own ROM branch at 80079E20. */
            case 0:
                break;
            case 1:
                parent = func_8007946C(u->side_12, u->slot_13);
                if (parent == 0 || parent->id_02 == -1 || !(parent->flags_49 & 2)) {
                    continue;
                }
                break;
            }

            offset = 0;
            a = 1.0f;
            b = 1.0f;
            c = 1.0f;
            d = 1.0f;
            if (u->type_00 == 0) {
                time = 0;
                if (func_800625FC(u->x_0C >> 7, u->z_10 >> 7) & 0x2000) {
                    time = u->timer_18;
                }
                if (u->flags_0A & 4) {
                    c = 1.1f;
                    d = 0.9f;
                }
                if (func_80042734(j) == 1 && u->y_0E < -16) {
                    switch (u->id_02) {
                    case 0x48: {
                        f32 angle = (f32)((time * 4) % 256) / 256.0f * TWO_PI;
                        b = func_80032360(angle) * 0.1 + 0.9;
                        b *= 0.85;
                        a = func_80027C60(angle) * 0.1 + 0.9;
                        a *= 0.85;
                        offset = -20;
                        break;
                    }
                    /* ODD_C: id 0x49 is the ROM's unscaled exception. */
                    case 0x49:
                        break;
                    /* ODD_C: the ROM jump table (0x8014CB48, 43 words) spans
                     * ids 0x20..0x4A and every entry except 0x48/0x49 selects
                     * this body. expand_end_case only emits a table when the
                     * case nodes reach CASE_VALUES_THRESHOLD and min/max are
                     * 0x20 and 0x4A; the original labels are not recoverable,
                     * so the observed range is listed. Measured: these 41
                     * labels 0; GNU range 0x20 ... 0x47 plus 0x4A 0 (rejected:
                     * the interfaces parser cannot read case ranges); range
                     * without 0x4A 500; only 0x20 and 0x4A 508; no labels
                     * 485; range extended with 0x4B 1. */
                    case 0x20: case 0x21: case 0x22: case 0x23:
                    case 0x24: case 0x25: case 0x26: case 0x27:
                    case 0x28: case 0x29: case 0x2A: case 0x2B:
                    case 0x2C: case 0x2D: case 0x2E: case 0x2F:
                    case 0x30: case 0x31: case 0x32: case 0x33:
                    case 0x34: case 0x35: case 0x36: case 0x37:
                    case 0x38: case 0x39: case 0x3A: case 0x3B:
                    case 0x3C: case 0x3D: case 0x3E: case 0x3F:
                    case 0x40: case 0x41: case 0x42: case 0x43:
                    case 0x44: case 0x45: case 0x46: case 0x47:
                    case 0x4A:
                    default: {
                        f32 angle = (f32)((time * 4) % 256) / 256.0f * TWO_PI;
                        b = func_80032360(angle) * 0.1 + 0.9;
                        b *= 0.85;
                        a = func_80027C60(angle) * 0.1 + 0.9;
                        a *= 0.85;
                        break;
                    }
                    }
                }
            } else if (u->type_00 == 3 && u->y_0E < -16) {
                b = func_80032360((f32)((u->timer_18 * 4) % 256) / 256.0f * TWO_PI) * 0.1 + 0.9;
                a = func_80027C60((f32)((u->timer_18 * 4) % 256) / 256.0f * TWO_PI) * 0.1 + 0.9;
            }

            if (u->rot_28 != 0.0 || u->rot_2C != 0.0 || u->rot_30 != 0.0) {
                func_8007991C(mf, u->rot_28, u->rot_2C, u->rot_30);
            } else {
                func_8002CE20(mf);
            }

            if (u->billboard_36 != 0 && u->attach_37 == 0) {
                switch (u->billboard_36) {
                case 1:
                    func_8002CBC0(mf, m0, mf);
                    break;
                case 2:
                    func_8002CBC0(mf, m1, mf);
                    break;
                case 4:
                    if (u->type_00 == 0) {
                        switch (u->id_02) {
                        case 0x1C:
                        case 0x1E:
                        case 0x53:
                        case 0x56:
                            func_8002CBC0(mf, r1, mf);
                            break;
                        case 0x49:
                            func_8002CBC0(mf, r2, mf);
                            break;
                        default:
                            func_8002CBC0(mf, m2, mf);
                            break;
                        }
                    } else {
                        func_8002CBC0(mf, m2, mf);
                    }
                    break;
                case 3:
                default:
                    func_8002CBC0(mf, m2, mf);
                    break;
                }
            }

            if (u->type_00 == 0) {
                info = func_80074784(u->id_02, u->level_06);
                height = (func_801E660C(j) + info->species->height_04 + offset) * u->scale_20;
            } else {
                height = u->desc_4C->table_0C.poses[u->frame_3F].lift_09 * u->scale_20;
            }

            if (u->attach_37 == 0) {
                x = u->x_0C * 0.25;
                z = u->z_10 * 0.25;
                if (u->raised_35) {
                    y = u->y_0E * 0.25 + height;
                } else {
                    y = u->y_0E * 0.25;
                }
            } else {
                x = u->x_0C;
                z = u->z_10;
                if (u->raised_35) {
                    y = u->y_0E + height;
                } else {
                    y = u->y_0E;
                }
            }
            func_80079698(mf, x, y + u->lift_14, z);
            func_80079800(mf, u->scale_1C * b * c, u->scale_20 * a * d, u->scale_24);

            if (u->type_00 == 0) {
                if ((u->x_0C >> 7) & 1) {
                    u->bob_34 = 1;
                } else {
                    u->bob_34 = -1;
                }
            }

            switch (u->attach_37) {
            case 0:
                func_80059B18(&view.raw);
                func_8002CBC0(mf, view.m, tmp);
                u->model.field34 = tmp[3][2];
                break;
            case 1:
                parent = func_8007946C(u->side_12, u->slot_13);
                if (parent == 0 || parent->id_02 == -1 || !(parent->flags_49 & 2) ||
                    parent->model.matrix2C == 0) {
                    continue;
                }
                func_8002CE20(tmp);
                func_8002CE80(tmp, parent->model.matrix2C);
                func_8002CBC0(mf, tmp, mf);
                u->model.field34 = parent->model.field34 - u->bob_34;
                break;
            }

            mtx = func_80070660(1);
            if (mtx == 0) {
                continue;
            }
            func_8002CD40(mf, mtx);
            u->model.matrix2C = mtx;
            u->flags_49 |= 2;
            *out++ = u;
            u->flags_49 |= 0x80;
        }
    }
    *out = 0;
}
/* ==== func_8007A680 ==== */

#define ABSF(v) ((v) >= 0.0f ? (v) : -(v))
#define DX(e) (cam.z - (f32)((e)->z_10 / 4))
#define DZ(e) (cam.x - (f32)((e)->x_0C / 4))
#define DIST(e) __builtin_sqrtf(ABSF(DZ(e)) * ABSF(DZ(e)) + ABSF(DX(e)) * ABSF(DX(e)))
void func_8007A680(void) {
    Unit *list[32];
    f32 mtx[4][4];
    Vec3f cam;
    Unit **far;
    Unit **q;
    Unit **best;
    s32 count = 0;
    Unit **p = list;
    s32 i;
    Unit *e;
    s32 step;
    SpeciesRef *info;
    for (i = 0; i < 30; i++) {
        /* ORIGINAL BUG: original call 0x8007A6D8 precedes the flags check.
         * A vacant -1 slot produces 0x8013F3C8. The inspected preceding
         * builder clears its 0x80 flag, so this loop does not dereference
         * that result; the guard does not make the pointer formation valid. */
        info = func_80074784(D_801DEAB4[i].id_02, D_801DEAB4[i].level_06);
        if ((D_801DEAB4[i].flags_49 & 0x80) && info->species->size_02 >= 0x3C) {
            *p++ = &D_801DEAB4[i];
            D_801DEAB4[i].slot_45 = i;
        }
    }
    far = p;
    for (i = 0; i < 30; i++) {
        /* ORIGINAL BUG: original call 0x8007A760 repeats the same -1
         * formation before the guard. Under the fresh-builder frame contract
         * the result is discarded without a table load, as in the first loop. */
        info = func_80074784(D_801DEAB4[i].id_02, D_801DEAB4[i].level_06);
        if ((D_801DEAB4[i].flags_49 & 0x80) && info->species->size_02 < 0x3C) {
            *p++ = &D_801DEAB4[i];
            D_801DEAB4[i].slot_45 = i;
        }
    }
    *p = 0;
    func_800593F8(&cam);
    for (p = far; *p != 0; p++) {
        best = p;
        for (q = p + 1; *q != 0; q++) {
            f32 d = DIST(*best);
            if (d > DIST(*q)) best = q;
        }
        if (best != p) {
            e = *best;
            *best = *p;
            *p = e;
        }
    }
    for (p = list; *p != 0; p++) {
        e = *p;
        step = e->desc_4C->table_0C.poses[e->frame_3F].step_0A;
        info = func_80074784(e->id_02, e->level_06);
        if (count < 8 && (info->species->size_02 >= 0x3C || (s32)e->model.field34 < D_8013D920)) {
            e->slot_45 = count;
            count++;
            e->show_44 = 1;
        } else if (step != 0) {
            e->show_44 = 0;
            e->frame_3F += step;
            func_8002CE80(mtx, e->model.matrix2C);
            func_80079800(mtx, 2.0f, 2.0f, 2.0f);
            func_8002CD40(mtx, e->model.matrix2C);
            e->flags_49 |= 1;
        } else if (count < 8) {
            e->slot_45 = count;
            count++;
            e->show_44 = 1;
        } else {
            e->show_44 = 0;
            func_80076044(e->slot_45, 0x4C, 2, 0, 0);
            e->frame_3F = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        Unit *t;
        e = &D_801D8FFC[i];
        if (e->id_02 == -1) continue;
        if (e->state_08 == 1) continue;
        if (e->fade46.v[1] == 0) continue;
        t = func_8007946C(e->side_12, e->slot_13);
        if (t == 0) continue;
        if (t->id_02 == -1) continue;
        step = e->desc_4C->table_0C.poses[e->frame_3F].step_0A;
        if (!(t->flags_49 & 1)) continue;
        if (step != 0) {
            e->frame_3F += step;
            func_8002CE80(mtx, e->model.matrix2C);
            func_80079800(mtx, 2.0f, 2.0f, 2.0f);
            func_8002CD40(mtx, e->model.matrix2C);
            e->flags_49 |= 1;
        }
    }
}

/* ==== func_8007AC64 ==== */
extern s32 D_8013D448;
extern s32 D_801D255C;
extern s32 D_801DEAAC;
extern s32 D_801DE9AC;
extern s32 D_801DE9B0;
extern s32 D_801E4E78;
extern Model D_801D4D20[];
extern Model D_801DFF84[];
extern u8 D_801E4E48[];
s32 func_8004217C(void);
s32 func_80070598(void *list, void *value);
s32 func_800706CC(Model *desc, s32 flag, Mtx *src, f32 dist, f32 angle, f32 height, f32 scale,
                  f32 scaleMul, s32 alpha, s32 mode);
s32 func_80070A0C(Model *model, Mtx *parent, f32 height, s32 frame, s32 alpha);
s32 func_80070D30(Model *model, Mtx *parent, f32 height, s32 frame, s32 alpha, s32 index, s32 flags);

/* Fade product scaled to 0..255. */
#define FADE(u) ((u)->fade46.v[0] * (u)->fade46.v[1] / 255)
/* One four-byte group of the color-combiner setup. */
#define COMBINE(u, i, a, b, c, d) \
    ((u)->model.combine1C[(i)] = (a), (u)->model.combine1C[(i) + 1] = (b), \
     (u)->model.combine1C[(i) + 2] = (c), (u)->model.combine1C[(i) + 3] = (d))

void func_8007AC64(void) {
    Unit **p;
    Unit *u;
    Unit *other;
    Zone *zone;
    SpeciesRef *gfx;
    s32 mode;
    s32 idx;
    f32 limit;
    s32 flags;
    s32 v;
    s32 maxv; /* brightness ceiling, reloaded every iteration */
    s32 alpha;
    f32 tilt;
    f32 lift;
    f32 ground;
    f32 h0;
    f32 scale;
    f32 size;
    s32 i;
    Unit *units;
    Unit *leader;

    D_8013D8C0 = D_801D4D20;
    D_8013D8C4 = D_801DFF84;
    idx = func_80074114();
    switch ((u32)func_800627C4()) {
    case 2:
        limit = -4.0f;
        mode = 0;
        break;
    case 4:
        limit = -8.0f;
        mode = 1;
        break;
    case 1:
        limit = -15.0f;
        mode = 0;
        break;
    case 3:
    default:
        limit = -8.0f;
        mode = 0;
        break;
    }

    for (p = D_801A76B0; *p != 0; p++) {
        (*p)->model.flags = 0;
        (*p)->model.kind = 1;
        (*p)->model.field2 = 0;
        (*p)->model.flags &= 0xF9;
        (*p)->model.matrix30 = 0;
        (*p)->model.flags = 0x10;
        if ((*p)->state_08 == 0) {
            (*p)->model.flags |= 1;
        }
        maxv = 0x60;
        if (D_8013D8D0 == 1) {
            (*p)->bright_48 = ((*p)->bright_48 + 0x18 < maxv) ? (*p)->bright_48 + 0x18 : maxv;
        } else {
            switch ((*p)->type_00) {
            case 3:
            case 5:
                flags = 0x4000;
                break;
            case 1:
            case 4:
                u = &D_801DEAB4[(*p)->slot_13];
                if (u->id_02 != -1) {
                    flags = func_800625FC(u->x_0C >> 7, u->z_10 >> 7);
                } else {
                    flags = 0;
                }
                break;
            case 2:
                flags = 0;
                break;
            case 0:
            default:
                flags = func_800625FC((*p)->x_0C >> 7, (*p)->z_10 >> 7);
                break;
            }
            if ((u8)func_80041FF8() == 0 && func_8004217C() == 0
                && ((flags & 0x4000) || ((flags & 0x20100000) && func_800627C4() == 3))) {
                (*p)->bright_48 = ((*p)->bright_48 + 0x18 < maxv) ? (*p)->bright_48 + 0x18 : maxv;
            } else {
                v = (*p)->bright_48 - 0x18;
                if (v < 0) {
                    v = 0;
                }
                (*p)->bright_48 = v;
            }
        }
        (*p)->model.field39 = (*p)->bright_48;
        (*p)->model.draw8 = 0x200005;
        (*p)->model.field4 = 0x100000;
        (*p)->model.field48 = (*p)->sprite_3C;
        (*p)->model.field4C = (*p)->frame_3F;
        (*p)->model.red50 = (*p)->palette_41;
        (*p)->model.green51 = (*p)->show_44;
        (*p)->model.blue52 = (*p)->slot_45;
        (*p)->model.field3C = 0;
        (*p)->model.field40 = 0;
        if ((*p)->attach_37 == 1 && (other = func_8007946C((*p)->side_12, (*p)->slot_13)) != 0
            && other->id_02 != -1) {
            (*p)->model.field44 = other->bob_34 + (*p)->bob_34 * 2;
        } else {
            (*p)->model.field44 = (*p)->bob_34;
        }

        if ((*p)->desc_4C->count_00 & 0x20) {
            switch ((*p)->sprite_3C) {
            case 2:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104F50;
                (*p)->model.color14.word = 0;
                (*p)->model.env18.word = (u8)FADE(*p);
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x114:
            case 0x181:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14.word = 0xF9DE7A00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0xD84E0000;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x13B:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14.word = 0xA09B9B00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0x3C323200;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x13A:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14.word = 0xFFFFFF00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0x73A0F100;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 3:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14.word = 0xFFFFFF00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0xFFFFFF00;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x162:
            default:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14.word = 0xFFFFFF00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0x10108F00;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            }
        } else {
            if ((*p)->fade46.pair == 0xFFFF) {
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x113078;
                (*p)->model.env18.rgba[3] = 0xFF;
                if ((*p)->model.env18.word & 0xFFFFFF00) {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 7, 7, 7, 1);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                } else {
                    COMBINE(*p, 0, 3, 1, 10, 1);
                    COMBINE(*p, 4, 7, 7, 7, 1);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                }
            } else {
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.env18.rgba[3] = FADE(*p);
                if ((*p)->model.env18.word & 0xFFFFFF00) {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 1, 7, 5, 7);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                } else {
                    COMBINE(*p, 0, 3, 1, 10, 1);
                    COMBINE(*p, 4, 1, 7, 5, 7);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                }
            }
            if (D_801DEAAC != 0) {
                /* Low bytes of the big-endian color words. */
                (*p)->model.env18.rgba[0] = ((u8 *)&D_801DE9AC)[3];
                (*p)->model.env18.rgba[1] = ((u8 *)&D_801E4E78)[3];
                (*p)->model.env18.rgba[2] = ((u8 *)&D_801DE9B0)[3];
                if ((*p)->fade46.pair == 0xFFFF) {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 7, 7, 7, 1);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                } else {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 1, 7, 5, 7);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                }
            }
        }

        func_80070598(D_801E4E48, &(*p)->model);

        if ((*p)->speed_38 != 0.0f) {
            switch ((*p)->type_00) {
            case 0:
                gfx = func_80074784((*p)->id_02, (*p)->level_06);
                tilt = (*p)->lift_14;
                if ((*p)->show_44 == 1) {
                    lift = gfx->species->lift_05;
                } else {
                    lift = gfx->species->lift_05 * 0.5f;
                }
                zone = &D_801A79E8[*p - D_801DEAB4];
                if (zone->count == 1) {
                    h0 = func_80076E74(zone->from[0].x, zone->from[0].y);
                    if (h0 == (f32)func_80076E74(zone->to[0].x, zone->to[0].y)) {
                        ground = h0;
                    } else {
                        ground = func_80076E74((*p)->x_0C >> 7, (*p)->z_10 >> 7);
                    }
                } else {
                    ground = func_80076E74((*p)->x_0C >> 7, (*p)->z_10 >> 7);
                }
                alpha = (*p)->fade46.v[1] * (*p)->fade46.v[0] / 255;
                size = (*p)->speed_38;
                scale = (*p)->scale_1C;
                if (func_800706CC(D_8013D8C0, mode, (*p)->model.matrix2C, tilt, ground, lift,
                                  size, scale, alpha, -1) == 0) {
                    func_80070598(D_801E4E48, D_8013D8C0);
                    D_8013D8C0++;
                }
                break;
            case 3:
            case 4:
                tilt = -(*p)->desc_4C->table_0C.poses[(*p)->frame_3F].lift_09;
                lift = tilt * 0.4f;
                ground = func_80076E74((*p)->x_0C >> 7, (*p)->z_10 >> 7);
                alpha = (*p)->fade46.v[1] * (*p)->fade46.v[0] / 255;
                scale = (*p)->scale_1C;
                switch ((*p)->sprite_3C) {
                case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: case 0x48: case 0x49:
                case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
                case 0x51: case 0x52:
                case 0x134:
                    size = (*p)->scale_1C;
                    if (func_800706CC(D_8013D8C0, mode, (*p)->model.matrix2C, tilt, ground, lift,
                                      size, scale, alpha, (*p)->sprite_3C) == 0) {
                        func_80070598(D_801E4E48, D_8013D8C0);
                        D_8013D8C0++;
                    }
                    break;
                default:
                    size = (*p)->speed_38;
                    if (func_800706CC(D_8013D8C0, mode, (*p)->model.matrix2C, tilt, ground, lift,
                                      size, scale, alpha, -1) == 0) {
                        func_80070598(D_801E4E48, D_8013D8C0);
                        D_8013D8C0++;
                    }
                    break;
                }
                break;
            }
        }

        if (D_8013D900 == 1 && (*p)->type_00 == 0
            && ((f32)((*p)->y_0E >> 2) <= limit || (*p)->id_02 == 0x54)
            && (func_800625FC((*p)->x_0C >> 7, (*p)->z_10 >> 7) & 0x2000)) {
            if (func_80070A0C(D_8013D8C0, (*p)->model.matrix2C, limit, (*p)->timer_18,
                              (*p)->fade46.v[1]) == 0) {
                func_80070598(D_801E4E48, D_8013D8C0);
                D_8013D8C0++;
            }
        }
    }

    /* Shadow under the leader unit. */
    units = D_801DEAB4;
    leader = &units[idx];
    if (leader->id_02 != -1 && D_801DEAB4[idx].state_08 != 1
        && D_801DEAB4[idx].model.env18.rgba[3] != 0) {
        u = leader;
        if (D_8013D8D8 == 1) {
            if (func_80070D30(D_8013D8C4, u->model.matrix2C, u->y_0E >> 2, u->timer_18, 0xFF,
                              u->dir_4A, 0) == 0) {
                func_80070598(D_801E4E48, D_8013D8C4);
                D_8013D8C4++;
            }
        } else if (D_8013D8DC == 1) {
            for (i = 0; i < 4; i++) {
                if (func_80070D30(D_8013D8C4, u->model.matrix2C, u->y_0E >> 2, u->timer_18, 0xFF,
                                  i * 2 + 1, 0) == 0) {
                    func_80070598(D_801E4E48, D_8013D8C4);
                    D_8013D8C4++;
                }
            }
        }
    }
    if (D_8013D448 != 0) {
        D_801D255C = D_801DEAB4[idx].model.field34;
    }
}

/* ==== func_8007BD30 ==== */
s32 func_8006D184(u8 id, u8 on);
void func_8007BF7C(void);
void func_8007AC64(void);

void func_8007BD30(void) {
    if (D_8013D8D4 != 0 && D_8013D8CC != 0) {
        func_8006D184(1, 0);
        func_80076264(D_8013D8E4);
        func_800795C8();
        func_80078908();
        func_800775F0();
        func_80075410();
        func_80077FE4();
        func_80079394();
        func_8007BF7C();
        func_80079A80();
        func_8007A680();
        func_8007AC64();
        D_8013D8C8 ^= 1;
        func_8006D184(1, 1);
    }
}

/* ==== func_8007BDE4 ==== */
void func_8007BDE4(void) {
    s32 i;
    EffectPair *e;
    for (i = 0; i < 16; i++) {
        e = &D_801A75F0[i];
        e->active = 0;
        e->timer = 0;
        e->ids[0] = -1;
        e->ids[1] = -1;
    }
}
/* ==== func_8007BE1C ==== */

s32 func_8007BE1C(s32 x, s32 y)
{
    s32 index;
    for (index = 0; index < 16; ++index) {
        EffectPair *entry = &D_801A75F0[index];
        if (!entry->active) {
            s32 id;
            Unit *unit;
            entry->active = 1;
            id = func_8007920C(0, 0x162, x, y);
            if (id != -1) {
                entry->ids[0] = id;
                unit = func_8007946C(4, id);
                unit->y_0E = func_80076EBC(x, y) * 4;
            } else {
                entry->ids[0] = id;
            }
            id = func_8007920C(0, 0x13A, x, y);
            if (id != -1) {
                entry->ids[1] = id;
                unit = func_8007946C(4, id);
                unit->y_0E = (func_80076EBC(x, y) + 1) * 4;
            } else {
                entry->ids[1] = id;
            }
            return index;
        }
    }
    return -1;
}

/* ==== func_8007BF38 ==== */
s32 func_8007BF38(s32 arg0) {
    EffectPair *entry;

    if (arg0 >= 16) {
        return -1;
    }
    entry = &D_801A75F0[arg0];
    entry->active = 0;
    entry->timer = 0;
    entry->ids[0] = -1;
    entry->ids[1] = -1;
    return arg0;
}
/* ==== func_8007BF7C ==== */

/* Animates the paired 0x162/0x13A effect units of each active entry through
 * their 9-frame timer, releasing each unit on frame 8 and the entry once both
 * are released. */
void func_8007BF7C(void) {
    s32 i;
    s32 k;
    EffectPair *entry;
    Unit *unit;

    for (i = 0; i < 16; i++) {
        entry = &D_801A75F0[i];
        if (entry->active != 1)
            continue;
        for (k = 0; k < 2; k++) {
            if (entry->ids[k] == -1)
                continue;
            unit = func_8007946C(4, entry->ids[k]);
            if (unit->id_02 == -1)
                continue;
            switch (unit->sprite_3C) {
            case 0x162:
                switch (entry->timer) {
                case 0:
                    unit->scale_1C = 1.0f;
                    unit->scale_20 = 0.4f;
                    unit->scale_24 = 1.0f;
                    unit->fade46.v[0] = 77;
                    unit->speed_38 = 0;
                    break;
                case 1:
                case 2:
                    unit->scale_1C = 0.85f;
                    unit->scale_20 = 0.7f;
                    unit->fade46.v[0] = 115;
                    break;
                case 3:
                case 4:
                    unit->scale_1C = 0.7f;
                    unit->scale_20 = 1.0f;
                    unit->fade46.v[0] = 153;
                    break;
                case 5:
                case 6:
                    unit->scale_1C = 0.55f;
                    unit->scale_20 = 0.7f;
                    unit->fade46.v[0] = 115;
                    break;
                case 7:
                    unit->scale_1C = 0.4f;
                    unit->scale_20 = 0.4f;
                    unit->fade46.v[0] = 77;
                    break;
                case 8:
                    func_8007935C(entry->ids[k]);
                    entry->ids[k] = -1;
                    break;
                }
                break;
            case 0x13A:
                switch (entry->timer) {
                case 0:
                    unit->rot_28 = 4.712389f;
                    unit->billboard_36 = 2;
                    unit->raised_35 = 0;
                    unit->speed_38 = 0;
                    unit->fade46.v[0] = 77;
                    unit->y_0E += 4;
                    unit->scale_1C = 0.5f;
                    unit->scale_20 = 0.5f;
                    unit->scale_24 = 0.5f;
                    break;
                case 1:
                case 2:
                    unit->fade46.v[0] = 115;
                    break;
                case 3:
                case 4:
                    unit->fade46.v[0] = 153;
                    break;
                case 5:
                case 6:
                    unit->fade46.v[0] = 115;
                    break;
                case 7:
                    unit->fade46.v[0] = 77;
                    break;
                case 8:
                    func_8007935C(entry->ids[k]);
                    entry->ids[k] = -1;
                    break;
                }
                break;
            }
        }
        entry->timer++;
        if (entry->ids[0] == -1 && entry->ids[1] == -1)
            func_8007BF38(i);
    }
}

/* ==== func_8007C1E0 ==== */
/* The only caller (func_8007C324) passes the direction un-narrowed; only the
 * halfword is stored. */
s32 func_8007C1E0(s32 index, s32 value)
{
    if (D_801DEAB4[index].id_02 != -1) {
        D_801DEAB4[index].dir_4A = value;
        return 0;
    }
    return -1;
}

/* ==== func_8007C224 ==== */
/* Returns -1/0 status; this caller discards it. */
s32 func_8007C410(s32 kind, s32 mode);
void func_8007C224(void){
    Unit *p;
    Unit *end;
    Zone *info;
    s32 kind;
    p = D_801DEAB4;
    end = &D_801DEAB4[29];
    for (; p <= end; p++) {
        info = &D_801A79E8[p - D_801DEAB4];
        kind = p->id_02;
        if (kind != -1 && info->count != 0 && (kind == 0x17 || kind == 0x1B))
            func_8007C410(kind, 0);
    }
}

/* ==== func_8007C324 ==== */
/* Interface note: this caller passes both values un-narrowed (daddu a0,s0 at
 * 0x8007C36C; a1 = full-width direction at 0x8007C3DC). func_80042944 narrows
 * its id itself (andi a0,0xFF at 0x80042950) and func_8007C1E0 only stores the
 * halfword, so both parameters are full-width s32 (canonical u8/s16 to be
 * corrected; see results.json). */
extern s32 func_80042944(s32 id);

/* Turns every placed unit one step (of 8 directions) towards the direction
 * func_80042944 reports, taking the shorter way round.
 * Returns -1 while disabled, 0 otherwise. */
s32 func_8007C324(void)
{
    s32 i;
    Unit *record;
    s32 target, current;

    if (!D_8013D8CC)
        return -1;
    for (i = 0; i < 30; i++) {
        record = &D_801DEAB4[i];
        if (record->id_02 == -1)
            continue;
        target = func_80042944(i);
        if (target == -1)
            continue;
        current = record->dir_4A;
        if (current == target)
            continue;
        if (current < target) {
            if (target - current < 5) {
                current++;
                target = 0;
                if (current < 8)
                    target = current;
            } else {
                target = current - 1;
                if (target < 0)
                    target = 7;
            }
        } else {
            if (current - target >= 5) {
                target = 0;
                current++;
                if (current < 8)
                    target = current;
            } else {
                target = current - 1;
                if (target < 0)
                    target = 7;
            }
        }
        func_8007C1E0(i, target);
    }
    return 0;
}
/* ==== func_8007C410 ==== */

/* Original returns -1 when disabled/unsupported and 0 after processing. */
s32 func_8007C410(s32 kind, s32 mode) {
    s32 index;
    if (!D_8013D8CC || (kind != 0x17 && kind != 0x1B)) return -1;
    for (index = 0; index < 30; index++) {
        Unit *actor = &D_801DEAB4[index];
        if (actor->id_02 != -1 && actor->id_02 == kind &&
            (!(actor->flags_0A & 0xBFFF) || actor->mode_05 == 1) &&
            actor->action_09 >= 3 && actor->active_07 != 2 && actor->mode_05 != mode) {
            s32 animation;
            s32 flags;
            actor->mode_05 = mode;
            if (mode == 1) {
                switch (actor->id_02) {
                case 0x17: animation = 0x185; flags = 3; break;
                case 0x1B: animation = 0x15B; flags = 0; break;
                default: animation = 0; flags = 3; break;
                }
            } else {
                switch (actor->id_02) {
                case 0x17: animation = 0; flags = 3; break;
                case 0x1B:
                    func_800751B4(index, actor->flags_0A & 0x4000);
                    animation = 0; flags = 3; break;
                default: animation = 0; flags = 3; break;
                }
            }
            func_80076044(index, animation, 1, 8, flags);
        }
    }
    return 0;
}