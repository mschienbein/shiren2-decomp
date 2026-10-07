#include "common.h"

typedef float f32;

typedef struct {
    char pad0[0x18];
    u32 data_18; /* PI ROM offset, not a CPU pointer. */
    char pad1C[0x18];
} Entry;

extern u32 D_8016DC00;
extern Entry D_8013C084[];
extern Entry *D_8013B984;
extern f32 *D_8013B804; /* float grid read by func_80062430/func_80062554 */
extern s32 D_8013B800;

extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);
/* Returns 0, D_8013B980->field_0C or D_801DFF7C plus a byte offset: the selected float grid. */
extern f32 *func_80064A70(u32 index);

void func_80066A28(u32 *entryIndex, u32 *subIndex) {
    u32 count;
    u32 index;

    if (*entryIndex >= 16) {
        *entryIndex = 0;
    }
    index = *entryIndex;
    D_8016DC00 = index;
    D_8013B984 = &D_8013C084[index];
    func_8006AAF0(&count, D_8013B984->data_18, 4);
    if (*subIndex >= count) {
        *subIndex = 0;
    }
    D_8013B804 = func_80064A70(*subIndex);
    D_8013B800 = 1;
}
