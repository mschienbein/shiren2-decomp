#include "common.h"
typedef struct Member Member;
typedef struct { unsigned char pad_00[0x8C]; Member *member_8C; } Obj;
extern s32 func_800CD278(Member *member);
s32 func_800F3998(Obj *obj) { return func_800CD278(obj->member_8C); }
