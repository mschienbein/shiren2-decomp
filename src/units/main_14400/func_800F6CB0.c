#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Embedded 0x18-byte collection; func_800CEC90 owns its layout (+0x04 methods, +0x08 entry
 * buffer, +0x0C capacity, +0x10 owner, +0x14 text id). */
typedef struct { u8 storage[0x18]; } Collection800F6CB0;
/* Partial object view: method table at +0x24, the collection's ten one-byte entries at
 * +0x80 (buffer handed to func_800CEC90) and the collection itself at +0x8C. */
typedef struct {
    u8 pad_00[0x24];
    const void *vtable_24;
    u8 pad_28[0x80 - 0x28];
    u8 entries_80[0x8C - 0x80];
    Collection800F6CB0 collection_8C;
} Object800F6CB0;
extern const unsigned char D_80159B08[152];
extern void *func_800F3CF0(void *obj, s32 kind, u8 arg);
extern void *func_800CEC90(void *object, void *owner, void *entries, u8 capacity, u16 text_id);
extern s32 func_800A3934(void *);
extern void func_800F6D30(void *, u8);
void *func_800F6CB0(Object800F6CB0 *object, u8 value) {
    func_800F3CF0(object, 0x5A, value);
    object->vtable_24 = D_80159B08;
    func_800CEC90(&object->collection_8C, object, object->entries_80, 10, 0x467);
    if (!func_800A3934(object)) func_800F6D30(object, value);
    return object;
}
