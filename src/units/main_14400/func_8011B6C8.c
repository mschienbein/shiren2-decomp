#include "common.h"
typedef unsigned char u8;
typedef struct { s32 field_00; s32 field_04; } Value;
typedef struct { s32 fields[4]; } Range;
typedef struct { s32 fields[8]; } Iterator;
extern void *func_800A2FD0(Range *range, Value *value, u8 radius);
extern Iterator *func_800A9244(Iterator *iterator, Range *range, Value *value);
extern s32 func_800A9284(Iterator *iterator, s32 kind);
extern void *func_800A942C(Iterator *iterator);
extern s32 func_800A692C(void *object, s32 kind);
extern s32 func_8011B5C0(void *object, void *entry);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 message_id, ...);
static inline void copy_value(Value *destination, Value *source) {
    destination->field_00 = source->field_00;
    destination->field_04 = source->field_04;
}
/* Item-effect slot +0x44 also supplies an item; this override does not use it. */
void func_8011B6C8(void *object, Value *input, void *item) {
    Value value;
    Range range;
    Iterator iterator;
    s32 success = 0;
    copy_value(&value, input);
    func_800A2FD0(&range, &value, 1);
    func_800A9244(&iterator, &range, &value);
    for (;;) {
        s32 active = func_800A9284(&iterator, 12);
        void *entry;
        s32 allowed;
        if (!active) break;
        entry = func_800A942C(&iterator);
        allowed = func_800A692C(entry, 10) != 1;
        if (allowed) success = (success | func_8011B5C0(object, entry)) != 0;
    }
    if (!success) {
        func_80049CB4(0x132);
        func_800498E4(0x222);
    }
}
