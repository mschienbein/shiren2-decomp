#include "common.h"

typedef unsigned char u8;
typedef u32 size_t;
typedef struct { u8 pad_00[0x17]; char text[0x50]; u8 field_67; } Obj;
/* Empty name returned for an out-of-range index: a single NUL char in .data,
   byte-aligned after the u8 globals D_801476D0/D_801476D1 (a char array would
   be word-aligned by GCC, the original object sits at 2 mod 4). */
char D_801476D2 = '\0';
extern size_t func_80032D70(const char *s);
static inline s32 entry_count(Obj *obj)
{
    return obj->field_67;
}
char *func_800CC74C(void *data, u8 index)
{
    Obj *obj = data;
    s32 offset;
    if ((s32)index >= entry_count(obj)) {
        return &D_801476D2;
    }
    offset = 0;
    while (index-- != 0) {
        s32 next = offset + 1;
        offset = next + (s32)func_80032D70((char *)obj + (offset + 0x17));
    }
    return (char *)obj + (offset + 0x17);
}
