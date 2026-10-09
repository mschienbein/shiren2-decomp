#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad00[0xC]; u8 flags0C; u8 field0D; u8 field0E; u8 field0F; u8 field10; } Dest;
typedef struct {
    u8 pad00[0xA]; u8 field0A; u8 pad0B[0x27]; u8 field32;
    u8 pad33[0x3F]; u8 flags72; u8 pad73[0x14]; u8 field87;
    u8 pad88[0x12]; u16 flags9A; u8 field9C; u8 field9D;
} Source;

void func_80128388(Dest *self, Source *source) {
    self->field0D = source->field0A;
    self->field0E = source->field32;
    self->field0F = source->field9D;
    self->field10 = source->field87;
    if (source->flags72 & 8) self->flags0C |= 1;
    if (source->flags9A & 0x40) self->flags0C |= 2;
    if (source->flags9A & 0x200) self->flags0C |= 4;
}
