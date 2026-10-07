#include "common.h"
typedef struct { s32 x,y,z; } Triple;
typedef struct { s32 field_0[3]; Triple field_C; float field_18, field_1C; } Object;
extern Triple D_80165324;
extern float D_8016533C;
extern void func_800593CC(Object *);
extern void func_80059668(float);
void func_800596CC(Object *arg) { func_800593CC(arg); D_80165324=arg->field_C; D_8016533C=arg->field_18; func_80059668(arg->field_1C); }
