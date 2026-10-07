#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 data[0x18];
} Entry800D3650;

extern Entry800D3650 D_80143330[2];
void func_800D2BE8(Entry800D3650 *entry, void *arg);

void func_800D3650(void *arg) {
    func_800D2BE8(&D_80143330[0], arg);
    func_800D2BE8(&D_80143330[1], arg);
}
