#include "common.h"
typedef unsigned char u8;
typedef void *(*Factory800AC244)(void);
extern s32 D_80143094[];
extern u8 D_8015374C[];
extern Factory800AC244 *D_80153860[];
extern s32 func_800AF950(s32 *state);
extern u8 func_800AC1AC(u8 id);
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_8010EA20(void *obj, s32 id);
extern void *func_8010CC70(void *obj, s32 id);
extern void *func_80113520(void *obj, s32 id);
extern void *func_8010DE30(void *obj, s32 id);
extern void *func_8010DD70(void *obj, s32 id);
extern s32 func_800AC670(void *obj);
static __inline__ s32 unavailable(void) {
    return func_800AF950(D_80143094) ^ 1;
}
void *func_800AC244(u8 id) {
    s32 item_id;
    u8 kind;
    s32 variant;
    void *obj;
    if (id != 0xF4 && id != 0xCE && id != 0xCF) {
        if (unavailable()) return 0;
    }
    item_id = id;
    kind = func_800AC1AC(item_id);
    variant = (u8)(item_id - D_8015374C[kind]);
    if (kind == 3) obj = func_8010EA20(func_800AC5B4(0x24, 0), item_id);
    else if (kind == 4) obj = func_8010CC70(func_800AC5B4(0x20, 0), item_id);
    else if (kind == 6) obj = func_80113520(func_800AC5B4(0x10, 0), item_id);
    else if (kind == 10) obj = func_8010DE30(func_800AC5B4(0xC, 0), item_id);
    else if (kind == 15) obj = func_8010DD70(func_800AC5B4(0x10, 1), item_id);
    else obj = D_80153860[kind][variant]();
    if (func_800AC670(obj)) return 0;
    return obj;
}
