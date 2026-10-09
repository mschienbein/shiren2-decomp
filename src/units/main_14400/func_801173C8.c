#include "common.h"

/* Complete 0x18-byte damage record; the constructor stores a source pointer at +0. */
typedef struct {
    void *source;
    u32 kind;
    s32 value;
    unsigned short amount, flags;
    unsigned char state;
    unsigned char reserved11[7];
} DamageRecord;

extern unsigned short D_801569D6;
extern s32 func_800E1CC4(void *, s32);
extern void func_80136910(DamageRecord *, void *, u32, u32, u32);
extern void func_800A7ADC(void *, DamageRecord *);

/* arg0 is unused but retained as the original virtual pair-action receiver. */
void func_801173C8(void *arg0, void *source, void *object) {
    DamageRecord record;
    DamageRecord *record_ptr;
    s32 scale = func_800E1CC4(object, 3) ? 2 : 1;
    record_ptr = &record;
    func_80136910(record_ptr, source, (short)(-(s32)D_801569D6 * scale), 0x21, 8);
    func_800A7ADC(object, record_ptr);
}
