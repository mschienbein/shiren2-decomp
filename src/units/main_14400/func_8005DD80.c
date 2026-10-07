#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 header;
    u8 data[0x86];
} Record;

extern Record D_80165980;
extern u8 D_00175150[];

/* devAddr is a PI ROM offset, not a CPU pointer. */
void func_8006AAF0(void *dst, u32 devAddr, s32 size);

void func_8005DD80(s32 id, u8 **data, s32 *kind, s32 *count) {
    Record *record = &D_80165980;

    func_8006AAF0(record, (u32)D_00175150 + id * 130, 0x88);
    *kind = record->header >> 8;
    *count = record->header & 0xFF;
    *data = record->data;
}
