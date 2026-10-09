#include "common.h"
typedef unsigned char u8;
/* Whole 0x20-byte controller cleared by func_8005DA84. */
typedef struct {
    short x0, y0, x1, y1;
    unsigned short x2, y2;
    u8 phase, mode;
    u8 level_c, target_c, level_b, target_b, level_a, target_a;
    u8 flags, reserved15, period, tick;
    u8 first, second, selection, reserved1B[5];
} RampState;
extern RampState D_80165960;
/* Address-only view of the 0x28-byte pool at .bss 0x801DE984 (func_8006EA64 and its siblings
 * access +0..+0x27); same element type as the other declarers. */
extern u8 D_801DE984[];
void func_8005CEDC(void);
void func_8005CE68(void);
unsigned char func_8005D24C(void **cursor);
void *func_8006EA64(void *cursor, void *end, void *pool);
void func_8005D700(void **p) {
    void *value = *p;
    if (D_80165960.flags == 0) return;
    if (D_80165960.flags & 0x20) {
        func_8005CEDC();
        func_8005CE68();
        value = func_8006EA64(value, 0, D_801DE984);
    } else {
        func_8005CEDC();
        func_8005CE68();
        if (func_8005D24C(&value)) {
            value = func_8006EA64(value, 0, D_801DE984);
        }
    }
    *p = value;
}
