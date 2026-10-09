#include "common.h"

typedef unsigned short u16;
typedef short s16;

typedef struct {
    u16 field_0;
    u16 field_2;
    char pad4[0x10 - 4];
    s16 field_10;
    char pad12[0x2C - 0x12];
} Rec80083568;

/* Ten 0x2C-byte records at 0x801A9080 (splat labels D_801A9080..D_801A9090 lie inside). */
extern Rec80083568 D_801A9080[10];

void func_80083568(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].field_2 >= 0x40) {
            D_801A9080[i].field_10 = 1;
        }
    }
}
