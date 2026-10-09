#include "common.h"
typedef struct { short adjust; unsigned short reserved; void (*call)(void *, s32, void *); } Entry;
typedef struct { unsigned char pad[0x18]; Entry *field18; } Object;
/* The two serialized bytes belong to the complete object, after its 0xC-byte base. */
typedef struct { unsigned char pad_00[0xC]; unsigned char fields_0C[2]; } SerializedObject;
extern unsigned char D_80160764[];
extern void func_800AF11C(void *, Object *);
extern void func_800CA4A4(Object *, void *);
void func_80128E24(SerializedObject *obj, Object *source) {
    func_800AF11C(obj, source);
    func_800CA4A4(source, D_80160764);
    source->field18[3].call((unsigned char *)source + source->field18[3].adjust, 2, obj->fields_0C);
}
