#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[2];
    s16 id;
    u8 pad4[0x50 - 4];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0xB0 - 0x5C];
} Actor80074174;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Slot80074174;

extern Actor80074174 D_801DEAB4[30];
extern Actor80074174 D_801D8FFC[4];
extern Actor80074174 D_801D2C2C[30];
extern Actor80074174 D_801DD378[32];
extern Actor80074174 D_801E02A8[110];
extern Slot80074174 D_801A7320[30];

void func_800765AC(void);
void func_8007BDE4(void);
void func_80074084(s32 arg);
void func_8007406C(s32 arg);
void func_80074078(s32 arg);
void func_80074090(s32 arg);
void func_800740A8(s32 arg);
void func_800740B4(s32 arg);
void func_800740C0(s32 arg);
void func_800740CC(s32 arg);
void func_800740E4(s32 arg);

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

static inline void resetActors(Actor80074174 *actor, s32 count, s32 full) {
    Actor80074174 *last = &actor[count - 1];

    for (; actor <= last; actor++) {
        actor->id = -1;
        if (full) {
            actor->unk50 = 0;
            actor->unk58 = 0;
            actor->unk54 = 0;
        }
    }
}

void func_80074174(s32 full) {
    Slot80074174 *slot;
    Slot80074174 *last;

    resetActors(D_801DEAB4, ARRAY_COUNT(D_801DEAB4), full);
    resetActors(D_801D8FFC, ARRAY_COUNT(D_801D8FFC), full);
    resetActors(D_801D2C2C, ARRAY_COUNT(D_801D2C2C), full);
    resetActors(D_801DD378, ARRAY_COUNT(D_801DD378), full);
    resetActors(D_801E02A8, ARRAY_COUNT(D_801E02A8), full);
    slot = D_801A7320;
    last = &slot[ARRAY_COUNT(D_801A7320) - 1];
    for (; slot <= last; slot++) {
        slot->unk0 = -1;
        slot->unk8 = -1;
        slot->unk10 = -1;
        slot->unk4 = 0;
        slot->unkC = 0;
        slot->unk14 = 0;
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
