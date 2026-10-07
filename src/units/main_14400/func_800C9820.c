#include "common.h"
/* Prefix of the record object at 0x80147680 (same view as func_800CAD44). Its +0x1C
 * child pointer (splat label D_8014769C) is stored by func_800CA76C at 0x800CA7A0
 * and cleared by func_800CA760. */
typedef struct RecordChild RecordChild;
typedef struct {
    unsigned char pad0[9];
    unsigned char flags9;
    signed char valueA;
    unsigned char padB[0x11];
    RecordChild *child1C;
} RecordObject;
extern RecordObject D_80147680;
extern void func_800CABF8(void *record, u32 first, u32 second);
extern void func_800CADBC(RecordObject *self);

/* Reads the record's child pointer; integrated at its call site. */
static inline RecordChild *recordChild(RecordObject *record) {
    return record->child1C;
}

void func_800C9820(s32 a, s32 b) {
    if (recordChild(&D_80147680)) {
        func_800CABF8(&D_80147680, (unsigned char)a, (unsigned char)b);
        func_800CADBC(&D_80147680);
    }
}
