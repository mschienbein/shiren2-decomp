#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos;
typedef struct Object Object;
typedef struct Iterator Iterator;
extern void func_800C2810(Iterator *iterator);
extern int func_800C28EC(const unsigned char *bytes);
extern void *func_800C28FC(void *out, void *iterator);
extern u32 func_800B1C6C(Pos *pos);
extern u8 D_8015441C[];
/* The member-call ABI supplies the receiver, but this calculation uses only the iterator. */
u32 func_800CF8E0(Object *unused, Iterator *iterator)
{
    Pos position;
    Pos next;
    u32 flags = 0;
    s32 index;
    func_800C2810(iterator);
    for (index = 0; func_800C28EC((u8 *)iterator) && index < 9; index++) {
        func_800C28FC(&next, iterator);
        position = next;
        if ((func_800B1C6C(&position) & 0x4000) || (flags & (1 << index))) {
            u8 value;
            s32 bit;
            s32 stride;
            s32 count;
            flags |= 1 << index;
            value = D_8015441C[index];
            bit = value & 127;
            stride = 1;
            if (value & 128) {
                stride = 2;
            }
            count = 2;
            if (stride == 1) {
                count = 3;
            }
            for (count--; count != -1; count--) {
                flags |= 1 << bit;
                bit += stride;
            }
        }
    }
    return flags;
}
