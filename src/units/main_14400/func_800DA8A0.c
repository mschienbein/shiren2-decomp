#include "common.h"

/* Eight-byte collection/item link at +8: constructed by func_800D0180, then copied from src. */
typedef struct {
    void *collection;
    void *item;
} Link;

typedef struct {
    short field_00;
    const void *field_04;
    Link field_08;
} Object;

extern const unsigned char D_80157FA8[]; /* 0x30-byte root command vtable. */
extern const unsigned char D_80158388[]; /* 0x30-byte command vtable. */
extern Link *func_800D0180(Link *link);

Object *func_800DA8A0(Object *self, s32 kind, Link *src) {
    self->field_04 = D_80157FA8;
    self->field_00 = kind;
    self->field_04 = D_80158388;
    func_800D0180(&self->field_08);
    self->field_08 = *src;
    return self;
}
