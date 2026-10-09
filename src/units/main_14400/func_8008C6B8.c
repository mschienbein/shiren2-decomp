#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field_0;
    u8 pad1[3];
    s32 field_4;
} Cursor;

/* 0x20-byte small slot: in-use flag, two work buffers and a cursor. */
typedef struct {
    u8 used;
    u8 pad1[7];
    u8 *buffer8;    /* 0x320 bytes */
    u8 padC[4];
    Cursor cursor;  /* 0x10 */
    u8 pad18[4];
    u8 *buffer1C;   /* 0x28 bytes */
} Slot;

u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_8008D6A0(Cursor *obj);
void *func_80091450(u32 arg);
void func_8008C75C(Slot *slot);

s32 func_8008C6B8(Slot *slot)
{
    s32 result = 0;

    func_8006A810((u8 *)slot, 0, sizeof(Slot));
    func_8008D6A0(&slot->cursor);
    slot->used = 1;
    /* ODD_C: single-pass error block routing either work-buffer allocation failure to the
     * func_8008C75C cleanup; it also keeps the used-flag store before the first allocation
     * call, matching the original scheduling. */
    do {
        if ((slot->buffer8 = func_80091450(0x320)) == 0) {
            result = -1;
            break;
        }
        func_8006A810(slot->buffer8, 0, 0x320);
        if ((slot->buffer1C = func_80091450(0x28)) == 0) {
            result = -1;
            break;
        }
        func_8006A810(slot->buffer1C, 0, 0x28);
    } while (0);
    if (result != 0) {
        func_8008C75C(slot);
    }
    return result;
}
