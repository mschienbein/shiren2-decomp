#include "common.h"
typedef struct { unsigned char field_0[0x24]; void *field_24; } Object;
extern s32 D_8015AFA8[];
extern void func_800EFD28(Object *,s32);
extern void func_800A3918(Object *);
void func_800FFD68(Object *arg,s32 flags) { arg->field_24=D_8015AFA8; func_800EFD28(arg,0); if(flags&1) func_800A3918(arg); }
