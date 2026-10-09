#include "common.h"

/* Stream vtable (e.g. D_8014A840): slot +0x20/+0x24 writes one word at a device offset;
 * its concrete target func_80044228 takes (self, unsigned long data, unsigned long offset). */
typedef struct {
    unsigned char pad_00[0x20];
    short adjust_20;
    short pad_22;
    void (*write_word_24)(void *self, unsigned long data, unsigned long offset);
} StreamVTable;

typedef struct {
    unsigned char pad_00[0x18];
    StreamVTable *vtable_18;
} Context;

typedef struct {
    unsigned char pad_00[0x18];
    s32 field_18;
    Context *field_1C;
} Object;

extern s32 func_800CB02C(s32);

void func_800CB050(Object *self)
{
    Context *context = self->field_1C;

    if (context) {
        s32 value;

        context->vtable_18->write_word_24((unsigned char *)context + context->vtable_18->adjust_20, self->field_18, 0x27F8);
        value = func_800CB02C(self->field_18);
        context = self->field_1C;
        context->vtable_18->write_word_24((unsigned char *)context + context->vtable_18->adjust_20, value, 0x27FC);
    }
}
