#include "common.h"
typedef struct { char pad[0x14]; s32 field14; } Obj;
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
typedef struct Unit Unit;
extern RoomState D_80143434;
extern s32 func_800D2A64(Obj *);
extern s32 func_800D2FB0(Obj *);
extern Unit *func_800D2B10(Obj *);
extern void func_800D3E28(s32, Unit *);
extern s32 func_800D49C0(void *);
extern s32 func_80045924(void);
extern s32 func_80049CB4(s32, ...);
s32 func_800D3510(Obj *p) {
    s32 state = func_800D2A64(p);
    if (func_800D2FB0(p) && (state || p->field14)) { if (state) func_800D3E28(1, func_800D2B10(p)); else func_800D3E28(0, 0); }
    else { s32 result = func_800D49C0(&D_80143434) ^ 1; if (result) func_80049CB4(294, func_80045924()); }
    return 1;
}
