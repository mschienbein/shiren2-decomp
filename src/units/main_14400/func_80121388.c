#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[8]; short delta_8; short pad_A; void (*field_C)(void *, s32); } ItemMethods;
typedef struct { u8 pad_0[8]; ItemMethods *methods; u8 field_C; u8 field_D; u8 field_E; } Item;
/* Item-set family D_80154300 slot +0x1C: void (void *self, s32 value). */
typedef struct { u8 pad_0[0x18]; short delta_18; short pad_1A; void (*field_1C)(void *self, s32 value); } ValueMethods;
/* List subobject: owner pointer, then its method table. */
typedef struct { void *owner_0; ValueMethods *methods; } Value;
typedef struct { u8 pad_0[0xC]; Value value; } S;
/* Published temporary pool record (0x30-byte stride pool); only tested for null here. */
typedef struct PoolRecord PoolRecord;
extern u8 D_80147620[];
extern PoolRecord *D_80143104;
extern u8 func_800C57A0(void *);
extern Item *func_800AAF00(void);
extern void *func_8011422C(u8 *);
extern s32 func_800CD5C0(void *, Item *);
extern void *func_800AC5B4(s32, s32);
extern void *func_80128280(void *);
extern s32 func_800AC670(Item *);
extern void func_800AA700(u8 *kind, u8 *level, u8 *excludedKinds);
extern void *func_80128F50(void *);
extern s32 func_800CD1FC(void *);
static inline void destroy_item(Item *item) { if (item) item->methods->field_C((u8 *)item + item->methods->delta_8, 3); }
static inline s32 insert_failed(void *list, Item *item) {
    return func_800CD5C0(list, item) ^ 1;
}
static inline s32 usable(Item *item) {
    return func_800AC670(item) ^ 1;
}
void func_80121388(S *self) {
    u8 kind, level;
    u8 roll = func_800C57A0(D_80147620);
    Item *item;
    if (!D_80143104) {
        if (roll < 90) {
            for (;;) {
                item = func_800AAF00();
                if (!item) break;
                if (insert_failed(func_8011422C((u8 *)self), item)) { destroy_item(item); break; }
            }
        } else if (roll < 180) {
            for (;;) {
                item = func_80128280(func_800AC5B4(20, 0));
                if (usable(item)) {
                    func_800AA700(&kind, &level, 0);
                    item->field_D = kind; item->field_E = level; item->field_C |= 1;
                    if (!kind || insert_failed(func_8011422C((u8 *)self), item)) { destroy_item(item); break; }
                }
                if (!usable(item)) break;
            }
        } else {
            item = func_80128F50(func_800AC5B4(12, 0));
            if (usable(item) && insert_failed(func_8011422C((u8 *)self), item)) destroy_item(item);
        }
        { s32 total = func_800CD1FC(func_8011422C((u8 *)self)); Value *value = &self->value;
          value->methods->field_1C((u8 *)value + value->methods->delta_18, total); }
    }
}
