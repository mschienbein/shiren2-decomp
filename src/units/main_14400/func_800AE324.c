#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;


typedef struct Unit800AE324 Unit800AE324;

extern u8 D_80156A09;

extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_800497F0(s32 id, ...);
extern void func_80049C90(s32 a, s32 b);
extern s32 func_800B5650(void *p);
extern char *func_800AC9D8(Item *obj);
extern s32 func_800B5300(Unit800AE324 *a, Unit800AE324 *b, u8 c);
extern void func_800AD868(Unit800AE324 *pos);
extern void func_800D3650(void *arg);

s32 func_800AE324(Item *item, Unit800AE324 *unit) {
    s32 usable = item->field08->test1C((u8 *)item + item->field08->delta18, 0x24) != 1;

    if (usable) {
        if (item->field08->test1C((u8 *)item + item->field08->delta18, 0x1F)) {
            func_80049CB4(0x11B, unit);
            func_800498E4(0x108);
            func_800B5650(unit);
        } else {
            s32 message = func_80049CB4(0x116, unit);

            func_800497F0(0x104, message, func_800AC9D8(item));
            func_80049C90(1, message);
            func_800497F0(0x107, message);
            func_800B5300(unit, 0, D_80156A09);
        }
        func_800AD868(unit);
        func_800D3650(item);
        if (item != 0) {
            item->field08->method0C((u8 *)item + item->field08->delta08, 3);
        }
        return 1;
    }
    return 0;
}
