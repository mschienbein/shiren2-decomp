#include "common.h"

typedef struct { void *field_00; void *field_04; } Entry800D01B8;
typedef struct Obj { unsigned short field_00; const void *field_04; Entry800D01B8 field_08; Entry800D01B8 field_10; } Obj;
/* 800DA8E0/800DA8E4 dereference argument 2 as a two-pointer entry. */
extern Obj *func_800DA8A0(Obj *obj, s32 kind, Entry800D01B8 *entry);
extern Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);
extern const unsigned char D_80158478[];
Obj *func_800DB3AC(Obj *obj, Entry800D01B8 *first, Entry800D01B8 *second)
{
    func_800DA8A0(obj, 0x10, first);
    obj->field_04 = D_80158478;
    func_800D0180(&obj->field_10);
    obj->field_10 = *second;
    return obj;
}
