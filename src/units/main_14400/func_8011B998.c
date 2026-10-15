#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Position;
typedef struct { void *field_0; short field_4; } Iterator;
extern Iterator *func_800B07F0(Iterator *);
extern s32 func_800B0808(Iterator *);
extern Item *func_800B0864(Iterator *);
extern void func_800D3650(void *);
extern s32 func_80049CB4(s32, ...);
extern void func_800497F0(s32, ...);
extern void func_800B4E7C(Position *);
extern void func_800A00C4(Position *, u8, void *, s32);
extern u8 D_80156A7B;
extern void *D_801476B8;
/* ODD_C: the masked flags go through a local and an inline != 0 so GCC 2.8.1 keeps
   andi + sne (sltu rd,$zero,rs) as the original does, instead of a bit extract. */
static inline s32 flag(Item *item) {
    s32 flags = item->flags02 & 0x20;
    return flags != 0;
}
static inline s32 selected(Item *item) {
    return item->field01 == 0x24 && flag(item);
}
void func_8011B998(void) {
    Iterator iterator; Position position;
    func_800B07F0(&iterator);
    while (func_800B0808(&iterator)) {
        Item *item = func_800B0864(&iterator);
        if (selected(item)) {
            s32 event;
            func_800D3650(item);
            position.x = item->payload.consumable.position10.x; position.y = item->payload.consumable.position10.y;
            event = func_80049CB4(0xDA, &position);
            func_800497F0(0x22B, event, func_800AE674(item));
            func_800B4E7C(&position);
            func_80049CB4(0xE1, &position);
            func_80049CB4(0xF5, &position);
            func_800A00C4(&position, D_80156A7B, D_801476B8, 0x16);
            func_80049CB4(0xD8, &position);
        }
    }
}
