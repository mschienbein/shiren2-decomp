#include "common.h"
typedef struct { unsigned char pad[0xC0]; s32 fieldC0; } Object;
extern void func_800EEED8(Object *);
void func_80109408(Object *obj) { func_800EEED8(obj); obj->fieldC0 = 0; }
