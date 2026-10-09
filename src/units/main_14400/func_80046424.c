#include "common.h"
/* Stream table D_801541F8 slot 0x18 targets func_800CA610. */
typedef struct {
    unsigned char pad_00[0x18];
    short adjustment_18;
    unsigned short reserved_1A;
    void (*write_1C)(void *, s32, const void *);
} StreamVTable80046424;
typedef struct { unsigned char pad_00[0x18]; StreamVTable80046424 *vtable_18; } Stream80046424;
typedef struct { unsigned char pad_00[3]; unsigned char field_03; } Obj80046424;
void func_80046424(Obj80046424 *self, Stream80046424 *stream) {
    unsigned char value = self->field_03;
    stream->vtable_18->write_1C((unsigned char *)stream + stream->vtable_18->adjustment_18, 1, &value);
}
