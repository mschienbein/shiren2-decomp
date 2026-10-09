#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 bytes[4];
} Bytes4;

/* Command object: base command copies the source link (func_800DA8A0); four bytes kept at +0x10. */
typedef struct {
    short field_00;
    const void *field_04;
    void *link_08[2];
    Bytes4 field_10;
} Object;

extern const unsigned char D_80158718[];
extern Object *func_800DA8A0(Object *self, s32 kind, void *src);

Object *func_800DCE60(Object *self, void *src, Bytes4 *bytes) {
    func_800DA8A0(self, 0x1E, src);
    self->field_04 = D_80158718;
    self->field_10 = *bytes;
    return self;
}
