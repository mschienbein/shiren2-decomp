#include "common.h"
typedef struct { short unk0; short pad2; void *vtbl4; void *unk8; } Obj;
typedef struct { char data[0x18]; } Entry18;
extern char D_80157FA8[], D_801580F8[];
extern Entry18 D_80143330[];
Obj *func_800D9D4C(Obj *o, unsigned char *idx){
    o->vtbl4 = D_80157FA8;
    o->unk0 = 0xB;
    o->vtbl4 = D_801580F8;
    { unsigned char i = *idx; Entry18 *t = D_80143330; o->unk8 = &t[i]; }
    return o;
}
