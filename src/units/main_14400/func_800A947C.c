#include "common.h"
typedef struct { s32 field_0; s32 *field_4; } Obj;
/* Partial view of the player object at 0x801C35E0: the sub-objects at 0xCC (0x24 bytes) and
 * 0xF0 (0x801C36D0) are the ones used here. The callees take the sub-object at 0xCC and the
 * whole object; both are addressed back from the 0xF0 member, which stays inside D_801C35E0
 * (the member-access spelling pl->subCC / pl does not reproduce the shared s0 base). */
typedef struct { char pad0[0xCC]; char subCC[0x24]; Obj objF0; } Player;
extern Player D_801C35E0;
extern s32 D_80154300[];
extern void func_800CE6A0(void *, s32), func_800E016C(void *, s32);
void func_800A947C(void) { Obj *p = &D_801C35E0.objF0; p->field_4 = D_80154300; func_800CE6A0((char *)p - 0x24, 2); func_800E016C((char *)p - 0xF0, 0); }
