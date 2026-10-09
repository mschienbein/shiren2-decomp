#include "common.h"
typedef unsigned char u8;
/* Render link at +0x48 of the destination object. */
typedef struct { void *unk0; void *unk4; u8 unk8; u8 unk9; u8 unkA; } Sub48;
typedef struct {
    u8 unk0; char pad1[0x3C - 1];
    s32 unk3C; s32 unk40; s32 unk44;
    Sub48 unk48;
    char pad54[0x1AC - 0x54];
    u8 unk1AC; char pad1AD[3];
    void *unk1B0;
} Actor;
/* EXTI record built by func_80090828: +0x18/+0x1C are the two loaded address words. */
typedef struct { char pad0[0x18]; void *unk18; void *unk1C; } Src;

/* Apply hook registered by func_80090828 through func_8008D3A0 (slot type
 * s32 (*)(void *record, void *owner, void *destination)); the owner pointer is
 * supplied by the caller and unused here. */
s32 func_800907C8(void *record, void *owner, void *destination) {
    Src *src = record;
    Actor *actor = destination;

    if (actor->unk0 == 1) {
        Sub48 *link = &actor->unk48;
        void *value = actor->unk1B0;

        if (value != 0) {
            link->unk0 = value;
        } else {
            link->unk0 = src->unk18;
        }
        link->unk4 = src->unk1C;
        /* ODD_C: the tag is read through the hook's destination argument rather than the
         * typed local; it names the same object and keeps that read on the incoming
         * argument register, as the original code does. */
        link->unk8 = ((Actor *)destination)->unk1AC;
        link->unk9 = 0xFE;
        link->unkA = 0;
        actor->unk3C = 0;
        actor->unk40 = 0;
        actor->unk44 = 0;
    }
    return 0;
}
