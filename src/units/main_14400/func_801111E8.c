#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 id, value; u8 pad_02[2]; const u8 *minimum, *maximum; } EffectRecord;
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    const u8 *unk4;
    const u8 *unk8;
    EffectRecord points[17];
    u8 tailD8[0xC];
} Table;
extern Table D_801484C0;
extern const u8 D_80156985;
extern const u8 D_80156D74[];
extern const u8 D_80156D88[], D_80156D9C[], D_80156DB0[], D_80156DC4[];
extern const u8 D_80156DD8[], D_80156E14[], D_80156DEC[], D_80156E00[];
extern const u8 D_80156EB4[], D_80156EF4[], D_80156F14[], D_80156F34[];
extern const u8 D_80156F54[], D_80156F74[], D_80156F94[], D_80156FB4[], D_80156FD4[];
/* Each original aggregate copy has its own 12-byte constant-record relocation. */
const EffectRecord D_8015D44C = {0x3B, 10, {0, 0}, D_80156D88, 0};
const EffectRecord D_8015D458 = {0x3C, 10, {0, 0}, D_80156D9C, 0};
const EffectRecord D_8015D464 = {0x3D, 10, {0, 0}, D_80156DB0, 0};
const EffectRecord D_8015D470 = {0x53, 10, {0, 0}, D_80156DC4, 0};
const EffectRecord D_8015D47C = {0x3E, 10, {0, 0}, D_80156DD8, 0};
const EffectRecord D_8015D488 = {0x56, 10, {0, 0}, D_80156E14, 0};
const EffectRecord D_8015D494 = {0x45, 0, {0, 0}, D_80156DEC, 0};
const EffectRecord D_8015D4A0 = {0x46, 0, {0, 0}, D_80156E00, 0};
const EffectRecord D_8015D4AC = {5, 0, {0, 0}, D_80156EB4, D_80156EB4};
const EffectRecord D_8015D4B8 = {1, 0, {0, 0}, D_80156EF4, D_80156EF4};
const EffectRecord D_8015D4C4 = {2, 0, {0, 0}, D_80156F14, D_80156F14};
const EffectRecord D_8015D4D0 = {14, 0, {0, 0}, D_80156F34, 0};
const EffectRecord D_8015D4DC = {9, 0, {0, 0}, D_80156F54, 0};
const EffectRecord D_8015D4E8 = {3, 10, {0, 0}, D_80156F74, 0};
const EffectRecord D_8015D4F4 = {6, 10, {0, 0}, D_80156F94, 0};
const EffectRecord D_8015D500 = {31, 0, {0, 0}, D_80156FB4, 0};
const EffectRecord D_8015D50C = {161, 0, {0, 0}, D_80156FD4, 0};
u8 *func_8006A810(void *, s32, s32);

void func_801111E8(void) {
    func_8006A810(&D_801484C0, 0, 0xC);
    D_801484C0.unk0 = 0x39;
    D_801484C0.unk4 = D_80156D74;
    D_801484C0.unk1 = D_80156985;
    D_801484C0.points[0] = D_8015D44C;
    D_801484C0.points[1] = D_8015D458;
    D_801484C0.points[2] = D_8015D464;
    D_801484C0.points[3] = D_8015D470;
    D_801484C0.points[4] = D_8015D47C;
    D_801484C0.points[5] = D_8015D488;
    D_801484C0.points[6] = D_8015D494;
    D_801484C0.points[7] = D_8015D4A0;
    D_801484C0.points[8] = D_8015D4AC;
    D_801484C0.points[9] = D_8015D4B8;
    D_801484C0.points[10] = D_8015D4C4;
    D_801484C0.points[11] = D_8015D4D0;
    D_801484C0.points[12] = D_8015D4DC;
    D_801484C0.points[13] = D_8015D4E8;
    D_801484C0.points[14] = D_8015D4F4;
    D_801484C0.points[15] = D_8015D500;
    D_801484C0.points[16] = D_8015D50C;
    func_8006A810(D_801484C0.tailD8, 0, 0xC);
}