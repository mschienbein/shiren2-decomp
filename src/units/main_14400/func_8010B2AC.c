#include "common.h"
/* Embedded list header: three pointers followed by capacity/start/count bytes. */
typedef struct {
    void *table_00;
    void *vtable_04;
    unsigned char *items_08;
    unsigned char capacity_0C, start_0D, count_0E;
} List;
typedef struct { unsigned char pad_00[0x24]; void *vtable_24; unsigned char pad_28[0x8C]; void *vtable_B4; unsigned char pad_B8[0x14]; List collection_CC; } Object8010B2AC;
extern unsigned char D_8015CB28[], D_8015CB48[], D_80159130[], D_80159150[];
s32 func_800EE598(void *object);
void func_800CD468(void *list);
void func_800CE6A0(List *list, s32 flags);
void func_800E016C(void *object, s32 flags);
void func_800A3918(void *object);
void func_8010B2AC(Object8010B2AC *object, s32 flags)
{
    object->vtable_B4 = D_8015CB28;
    object->vtable_24 = D_8015CB48;
    func_800EE598(object);
    func_800CD468(&object->collection_CC);
    func_800CE6A0(&object->collection_CC, 2);
    object->vtable_B4 = D_80159130;
    object->vtable_24 = D_80159150;
    func_800E016C(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
