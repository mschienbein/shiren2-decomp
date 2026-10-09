#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 x, y; } Position;
typedef Position Value;
typedef struct { u8 pad00[0x18]; s16 delta18, index1A; void (*reset1C)(void *); } VTable;
typedef struct { Position position; u8 pad08[0x14]; u16 flags1C; u8 pad1E[6]; VTable *vtable24; } Object;
typedef Object Pair800A59A4;
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A59A4(Pair800A59A4 *self);
extern s32 func_800A5D2C(void *object, Value *output, s32 flags);
extern s32 func_800A60D8(Object *self, Position *position);
extern void func_800A58FC(void *actor, Position *position);
extern void func_800A7BA4(Object *, s32);

static __inline__ Position *assign_position(Position *out, const Position *in) {
    *out = *in;
    return out;
}

s32 func_800A5168(Object *self, Position *position, s32 alternate) {
    Position old, next;
    Position *previous = &old;
    Position *dest;
    s32 message;
    old.x = self->position.x;
    previous->y = self->position.y;
    if (self->flags1C & 2) {
        func_80049CB4(0x10A8, self);
        func_80049CB4(0x14, self, previous);
        func_80049CB4(0x1F, self);
        return 1;
    }
    next.x = position->x;
    next.y = position->y;
    func_800A59A4(self);
    dest = &next;
    {
        s32 failed = func_800A5D2C(self, dest, 10);
        failed ^= 1;
        if (failed) {
            failed = func_800A60D8(self, dest);
            failed ^= 1;
            if (failed) {
                self->vtable24->reset1C((u8 *)self + self->vtable24->delta18);
                return 0;
            }
        }
    }
    assign_position(&self->position, &next);
    message = 0x1C;
    if (alternate) message = 0x1D;
    func_80049CB4(message | 0x1000, self, &old);
    func_800A58FC(self, &next);
    func_800A7BA4(self, 2);
    return 1;
}
