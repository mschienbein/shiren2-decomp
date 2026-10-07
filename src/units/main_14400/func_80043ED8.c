#include "common.h"

typedef struct Obj Obj;
/* gcc 2.x vtable entry: this-delta, index, function */
typedef struct { short delta; short index; void (*fn)(void *); } VEntry;
struct Obj {
    s32 pos;            /* 0x00 */
    char pad4[0x14];
    VEntry *vtbl;       /* 0x18 */
    u32 base;           /* 0x1C: PI device address of the stream */
    char buf[0x20];     /* 0x20 */
    s32 cached;         /* 0x40 */
};
extern void func_80043C2C(u32 deviceAddress, void *destination, u32 length);
void func_80043ED8(Obj *o) {
    if (o->cached >= 0) {
        if (o->pos < o->cached || o->cached + 24 < o->pos) {
            o->vtbl[1].fn((char *)o + o->vtbl[1].delta);
        } else {
            return;
        }
    }
    o->cached = o->pos & ~1;
    func_80043C2C(o->base + o->cached, o->buf, 0x20);
}
