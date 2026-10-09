#include "common.h"
typedef unsigned char u8;
typedef struct { unsigned char pad0[0x18]; short adjust_18; short pad1A; void (*call_1C)(void *, s32, void *); } VTable;
typedef struct { unsigned char pad0[0x18]; VTable *field_18; } Target;
typedef struct { unsigned char pad0[0x10]; u8 field_10, field_11; } Object;
extern const char D_8015ED7C[16];
void func_800CA4A4(void *self, void *text);
void func_801117B4(char *self, Target *target);
void func_8011DA4C(Object *self, Target *target) {
    u8 value;
    VTable *table;
    func_800CA4A4(target, (void *)D_8015ED7C);
    func_801117B4((char *)self, target);
    if (self->field_10 == 0) value = 0;
    else value = self->field_10 & 0x7F;
    value |= (self->field_11 & 1) << 7;
    table = target->field_18;
    table->call_1C((unsigned char *)target + table->adjust_18, 1, &value);
}
