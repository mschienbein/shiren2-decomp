#include "common.h"
typedef unsigned short u16;

typedef struct { char pad[0xA]; signed char fieldA; } Object;
extern unsigned short D_80154238[];
extern Object *func_800C9E10(void);
extern s32 func_80094E6C(s32);
extern char *func_80048480(u16);
extern void func_800CB268(Object *,s32),func_800CADBC(Object *);
extern void func_80048240(u16,...);
void func_80094F30(void) { Object *obj=func_800C9E10(); s32 index=func_80094E6C(obj->fieldA); if(index<3) { func_800CB268(obj,index); func_800CADBC(obj); func_80048240(0x28F,func_80048480(D_80154238[index])); } }
