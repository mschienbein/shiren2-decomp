#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef float f32;
typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0xA9];
} Member;
typedef struct {
    u8 pad0[2];
    u8 value;
} Stats;
typedef struct {
    s16 slot0;
    s16 unk2;
    s16 slot1;
    s16 unk6;
    s16 flag0;
    s16 flag1;
} Equip;
/* Equipment descriptor (4 bytes): +0 model/tile id (lhu), +2 percentage
 * byte (lbu), +3 never accessed. */
typedef struct { u16 id; u8 percent; u8 unk3; } EquipInfo;
typedef struct {
    s16 unk0;
    u8 pad2[5];
    u8 unk7;
    u8 pad8[0xA];
    u8 unk12;
    u8 unk13;
    u8 pad14[8];
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    u8 pad28[0xF];
    u8 unk37;
    u8 pad38[0xC];
    u8 unk44;
    u8 unk45;
    u8 pad46[0x6A];
} Model;
extern Member D_801DEAB4[];
extern Equip D_8013D904[];
/* Equipment descriptor tables indexed by kind minus the first kind of each
 * class. They are separate symbols in the original: the index bias is computed
 * at run time (addiu -0x32/-0xAA/-0x59 before the shift) instead of being
 * folded into a base address, which GCC does for an offset into one array. */
extern EquipInfo D_8013F24C[];
extern EquipInfo D_8013F2E8[];
extern EquipInfo D_8013F2EC[];
extern Model D_801D8FFC[];
s32 func_80041C64(s32 who);
/* Returns the kind/level entry, whose first word points at the stats. */
Stats **func_80074784(s32 kind, s32 level);
void func_80074778(Model *model);
s32 func_80074500(Model *models, s32 index, s32 count, s32 kind, s32 resource);

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
    Member *leader;
    Model *m;

    member = func_80041C64(who);
    if (member == -1) {
        return -1;
    }
    leader = D_801DEAB4;
    if (leader[member].unk2 == -1) {
        return -1;
    }
    switch (who) {
    case 0x17:
        party = 0;
        break;
    case 0x1A:
        party = 1;
        {
            f32 own = (*func_80074784(0x17, leader->unk6))->value / 100.0f;
            ratio = own / ((*func_80074784(0x1A, D_801DEAB4[member].unk6))->value / 100.0f);
        }
        break;
    default:
        return -1;
    }
    equip = &D_8013D904[party];
    switch (slot) {
    case 3:
        index = party * 2;
        equip->slot0 = id;
        equip->flag0 = 0;
        kind = equip->slot0;
        if (kind != -1) {
            extra = D_8013F24C[kind - 0x32].id;
            scale = D_8013F24C[kind - 0x32].percent / 100.0f;
            scale *= ratio;
        }
        if (kind == 0x3A || kind == 0x53 || kind == 0x54 || kind == 0x55) {
            equip->flag0 = 1;
        }
        break;
    case 9:
        index = party * 2;
        equip->slot0 = id;
        equip->flag0 = 0;
        kind = equip->slot0;
        if (kind != -1) {
            extra = D_8013F2E8[kind - 0xAA].id;
            scale = D_8013F2E8[kind - 0xAA].percent / 100.0f;
            scale *= ratio;
        }
        break;
    case 4:
        index = party * 2 + 1;
        equip->slot1 = id;
        equip->flag1 = 0;
        kind = equip->slot1;
        if (kind != -1) {
            extra = D_8013F2EC[kind - 0x59].id;
            scale = D_8013F2EC[kind - 0x59].percent / 100.0f;
            scale *= ratio;
        }
        if (kind == 0x59 || kind == 0x61 || kind == 0x5E) {
            equip->flag1 = 1;
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
        m->unk0 = 1;
        m->unk37 = 1;
        m->unk12 = 0;
        m->unk13 = member;
        m->scaleX = scale;
        m->scaleY = scale;
        m->scaleZ = scale;
        m->unk44 = 2;
        m->unk45 = index;
        m->unk7 = 1;
        equip->unk6 = 0;
    }
    return index;
}
