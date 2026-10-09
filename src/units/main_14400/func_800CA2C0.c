#include "common.h"
typedef unsigned char u8;
typedef struct { char pad0[8]; short adjust8; short padA; void (*flush)(void *); char pad10[8]; short adjust18; short pad1A; void (*write)(void *, s32, void *); char pad20[8]; short adjust28; short pad2A; void (*read)(void *, s32, void *); } VTable;
typedef struct Object { s32 position; s32 end; s32 checksum; char padC[0xC]; VTable *vtable; } Object;
void func_800CA0A8(Object *object, s32 value);
void func_800CA0EC(Object *, s32 *, u8);
void func_800CA1BC(Object *object);
static inline s32 positive(s32 value) { return value > 0; }
void func_800CA2C0(void *arg0) {
    Object *object = arg0;
    s32 position = object->position;
    s32 remaining;
    u8 byte;
    if (object->end < position) {
        remaining = position - object->end;
        func_800CA0A8(object, object->end);
        object->end = position;
        while (positive(remaining--)) {
            object->vtable->read((char *)object + object->vtable->adjust28, 1, &byte);
            func_800CA0EC(object, &object->checksum, byte);
        }
        func_800CA0A8(object, 0);
        object->vtable->write((char *)object + object->vtable->adjust18, 4, &object->end);
        func_800CA0A8(object, 4);
        object->vtable->write((char *)object + object->vtable->adjust18, 4, &object->checksum);
    } else {
        func_800CA0A8(object, 0);
        object->vtable->write((char *)object + object->vtable->adjust18, 4, &object->end);
        func_800CA1BC(object);
    }
    object->vtable->flush((char *)object + object->vtable->adjust8);
    func_800CA0A8(object, position);
}
