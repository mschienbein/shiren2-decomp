#include "common.h"

typedef unsigned char u8;
typedef struct Object Object;
typedef struct RecordView_800C2810 RecordView_800C2810;
typedef RecordView_800C2810 Iterator;
typedef struct { s32 x, y; } Pos;
extern u8 D_80154428[16];
extern void func_800C2810(RecordView_800C2810 *record);
extern int func_800C28EC(const unsigned char *bytes);
extern void *func_800C28FC(void *out, void *iterator);
extern u32 func_800B1C6C(Pos *pos);

/* The member-call ABI supplies object; only iterator is read by this calculation. */
u32 func_800CF9E4(Object *object, Iterator *iterator) {
    Pos position;
    Pos next;
    u8 *codes = D_80154428;
    u32 result = 0;
    s32 index = 0;
    func_800C2810(iterator);
    while (func_800C28EC((const u8 *)iterator) && index < 10) {
        s32 count;
        func_800C28FC(&next, iterator);
        position = next;
        count = *codes >> 6;
        if ((func_800B1C6C(&position) & 0x4000) || (result & (1 << index))) {
            result |= 1 << index;
            while (--count != -1) {
                /* Table bytes select bits 0..31 by their low five bits;
                 * the original sllv at 0x800CFAA0 masks the shift count. */
                result |= 1 << *codes++;
            }
        } else {
            codes += count;
        }
        index++;
    }
    return result;
}
