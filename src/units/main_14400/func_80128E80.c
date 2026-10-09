#include "common.h"
typedef short s16;
typedef struct { char unk0[0x28]; s16 unk28, unk2A; void (*unk2C)(void *, s32, void *); } Dispatch;
typedef struct { char unk0[0x18]; Dispatch *unk18; } State;
/* The read destination is the two-byte tail of the complete 0xE-byte object. */
typedef struct { unsigned char pad_00[0xC]; unsigned char fields_0C[2]; } Object;
extern unsigned char D_80160764[];
extern void func_800AF174(Object *, State *);
extern void func_800CA4E8(State *, void *);
void func_80128E80(Object *arg0, State *arg1) {
    Dispatch *dispatch;
    func_800AF174(arg0, arg1);
    func_800CA4E8(arg1, D_80160764);
    dispatch = arg1->unk18;
    dispatch->unk2C((char *)arg1 + dispatch->unk28, 2, arg0->fields_0C);
}
