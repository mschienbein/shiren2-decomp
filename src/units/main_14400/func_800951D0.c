#include "common.h"

/* Embedded 0x10-byte buffer; func_80048728 reads and clears its handle at +0xC. */
typedef struct { unsigned char pad00[0xC]; s32 handle0C; } Buffer;
typedef struct {
    s32 *vtbl_0;
    Buffer field_4;
} Obj_800951D0;

extern s32 D_80151E10[];
extern void func_80048728(void *);
extern void func_800D8FA8(void *object);

void func_800951D0(Obj_800951D0 *self, s32 flags) {
    self->vtbl_0 = D_80151E10;
    func_80048728(&self->field_4);
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
