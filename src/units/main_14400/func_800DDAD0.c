#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { void *field_0; void *field_4; } Entry800D01B8;
typedef struct { u16 field_0; u16 pad_2; void *field_4; } Base;
typedef struct { u32 field_0; void *field_4; Entry800D01B8 *field_8; s32 field_C; s32 field_10; } List;
typedef struct { Base base; Entry800D01B8 entries[21]; List field_B0; } S;
extern u8 D_80157FA8[];
extern u8 D_801588F8[];
extern u8 D_801545E0[];
extern Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);
extern void func_800D03B4(unsigned char *object, void *entries, s32 count);
static __inline__ void initialize_base(Base *base, s32 value)
{
    base->field_4 = D_80157FA8;
    base->field_0 = value;
}
static __inline__ void initialize_list(List *list, Entry800D01B8 *entries)
{
    list->field_4 = D_801545E0;
    func_800D03B4((u8 *)list, entries, 21);
}
S *func_800DDAD0(S *object, s32 value)
{
    Entry800D01B8 *entry = object->entries;
    s32 count = 20;
    initialize_base(&object->base, value);
    object->base.field_4 = D_801588F8;
    do {
        func_800D0180(entry);
        count--;
        entry++;
    } while (count != -1);
    initialize_list(&object->field_B0, object->entries);
    return object;
}
