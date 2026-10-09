#include "common.h"

typedef unsigned char u8;

/* One-byte direction value (low three bits of the D_80148790 table word). */
typedef struct {
    u8 value;
} Dir;

typedef struct {
    u8 pad00[8];
    void *vtable;
    u8 pad0C[4];
    Dir dir;
} Obj80126930;

extern Obj80126930 *func_80115690(Obj80126930 *obj, s32 kind);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern u8 D_80147620[];
extern char D_80160480[];
extern s32 D_80148790[4];

/* In-place initializer of a direction temporary. */
static inline void dir_init(Dir *dir, s32 value) {
    dir->value = value & 7;
}

/* Constructor: base kind 0xE7, own vtable, random initial direction. */
Obj80126930 *func_80126930(Obj80126930 *obj) {
    Dir dir;

    func_80115690(obj, 0xE7);
    obj->vtable = D_80160480;
    dir_init(&dir, D_80148790[(u8)func_800C5844(D_80147620, 0, 3)]);
    obj->dir = dir;
    return obj;
}
