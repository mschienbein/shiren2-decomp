#include "common.h"
typedef unsigned char u8;
typedef struct Object Object;
typedef struct Owner Owner;
typedef struct Target Target;
struct Object { s32 kind; s32 id; void (*field_8)(void); s32 (*field_C)(Object *, Owner *, Target *); void (*field_10)(void *); s32 field_14; u8 field_18; u8 field_19; u8 pad_1A[2]; s32 field_1C; s32 field_20; s32 field_24; s32 field_28; s32 field_2C; };
typedef struct { u8 pad_0[2]; u8 count; u8 pad_3[5]; Object **items; } Table;
extern void *func_80091450(u32);
extern u8 *func_8006A810(u8 *, s32, s32);
extern void func_8008D3A0(Object *, s32, void (*)(void), s32 (*)(void *, void *, void *), void (*)(void *));
extern void func_8008EFA0(void);
extern void func_8008F094(void *);
extern s32 func_8008EFA8(void *record, void *owner, void *destination);
extern s32 func_8008DF04(void *);
extern u32 func_8008E0C4(void *, void *, u32);
extern void func_80091544(void *);
static inline void clear_object(Object *object) {
    func_8006A810((u8 *)object, 0, sizeof(*object));
}
s32 func_8008F09C(void *stream, s32 length, Table *table) {
    s32 result = 0;
    Object *object;
    /* ODD_C: single-pass error block grouping object allocation and field/length validation
     * before the shared cleanup; it also shapes the prologue scheduling. */
    do {
        s32 expected;
        object = func_80091450(sizeof(Object));
        if (!object) { result = -1; break; }
        clear_object(object);
        func_8008D3A0(object, 0x4F424A43, func_8008EFA0, func_8008EFA8, func_8008F094);
        object->field_14 = func_8008DF04(stream);
        func_8008E0C4(stream, &object->field_18, 1);
        func_8008E0C4(stream, &object->field_19, 1);
        object->field_1C = func_8008DF04(stream);
        object->field_24 = func_8008DF04(stream);
        expected = 0xE;
        if (object->field_24) { object->field_28 = func_8008DF04(stream); expected = 0x12; }
        if (length != expected) { result = -1; break; }
        table->items[table->count++] = object;
    } while (0);
    if (result && object) func_80091544(object);
    return result;
}
