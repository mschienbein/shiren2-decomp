#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 delta; s16 index; void *(*fn)(void *self); } VtblEntry800A6DE4;
typedef struct { u8 pad0[0x98]; VtblEntry800A6DE4 entry; } Vtbl800A6DE4;
typedef struct {
    u8 pad0[0xA];
    u8 kind;
    u8 padB[0x13];
    u8 flags;
    u8 pad1F[0x5];
    Vtbl800A6DE4 *vtable;
    u8 pad28[0x64];
    void *field_8C;
} Obj800A6DE4;
void *func_800A6DE4(Obj800A6DE4 *obj) {
    u8 flags = obj->flags;
    if (flags & 0xC) {
        return obj->vtable->entry.fn((u8 *)obj + obj->vtable->entry.delta);
    }
    if ((flags >> 4) & 1) {
        return obj->field_8C;
    }
    if (obj->kind == 0x5A) {
        return &obj->field_8C;
    }
    return 0;
}
