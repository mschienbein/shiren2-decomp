#include "common.h"

typedef unsigned char u8;

extern "C" {

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
} Obj800EA3F0;

extern s32 D_80148090;
extern u8 D_80147620[];
extern const u8 D_8015693F;
extern const u8 D_80156941;
extern const u8 D_80156943;
extern const u8 D_80156945;
extern const u8 D_80156947;
extern const u8 D_80156949;
extern const u8 D_8015694B;
extern const u8 D_8015694D;
extern const u8 D_8015694F;
extern const u8 D_80156951;
extern const u8 D_80156953;
extern const u8 D_80156955;
extern const u8 D_80156957;
extern const u8 D_8015695B;
extern const u8 D_8015695F;
extern const u8 D_80156961;
extern const u8 D_801569F3;
extern const u8 D_80156A7D;
extern const u8 D_80156A7F;
extern const u8 D_80156A85;
extern const u8 D_80156A87;

extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800E11C0(void *obj, s32 index, u8 value, s32 arg);

void func_800EA3F0(Obj800EA3F0 *obj, s32 index, u8 value, s32 arg) {
    /*
     * Per-index value ranges { lower, upper } taken from the read-only item
     * parameter entries; upper 0 means the lower value is used as is.  A g++
     * function-local static with dynamic initializers: filled on first use
     * under its guard word (the .data word at 0x80148350; the table is the
     * .bss array at 0x801C9EE0) and read-only afterwards.
     */
    static u8 value_range[21][2] = {
        { D_80156945, 0 },
        { D_80156947, 0 },
        { D_80156949, 0 },
        { D_801569F3, 0 },
        { D_8015695B, 0 },
        { D_8015695F, D_80156961 },
        { D_8015695F, D_80156961 },
        { D_8015695F, D_80156961 },
        { D_8015695F, D_80156961 },
        { D_80156951, 0 },
        { D_80156941, 0 },
        { D_80156943, 0 },
        { D_8015694B, 0 },
        { D_8015694D, 0 },
        { D_8015694F, 0 },
        { D_80156A7D, D_80156A7F },
        { D_80156A85, D_80156A87 },
        { D_80156953, 0 },
        { D_80156955, 0 },
        { D_8015693F, 0 },
        { D_80156957, 0 }
    };
    if (value == 0xFE) {
        if (value_range[index][1] == 0) {
            value = value_range[index][0];
        } else {
            value = func_800C5844(D_80147620, value_range[index][0], value_range[index][1]);
        }
    }
    func_800E11C0(obj, index, value, arg);
    if ((obj->flags_1E >> 2) & 1) {
        D_80148090 = 0;
    }
}

}
