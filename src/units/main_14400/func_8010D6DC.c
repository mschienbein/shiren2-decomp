#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
/* Shared item view: func_8010C174 reads the modifier and all sixteen items. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xD - 2];
    s8 modifier;
    u8 capacity;
    u8 count;
    u8 items[16];
} Obj;
extern u16 D_801574AC[], D_80157524[], D_801571AC[], D_80157280[], D_801575EC[], D_80157734[], D_80157670[];
extern u8 D_801575A0[];
s32 func_8010C174(Obj *, u8, u16 *, u8 *, u16 *, u16 *, u16 *, u16 *, u16 *, u16 *);
s32 func_8010D6DC(Obj *o){
    return func_8010C174(o, o->unk1, D_801574AC, D_801575A0, D_80157524, D_801571AC, D_80157280, D_801575EC, D_80157734, D_80157670);
}
