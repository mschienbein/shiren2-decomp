#include "common.h"

typedef struct {
    char pad0[0x4C];
    const void *vtable;
    char pad50[0x18];
    s32 field_68;
    void *field_6C;
    char pad70[0xA0];
} Obj;
typedef struct {
    s32 value;
    char pad4[0x1C];
} Result;

extern char D_80138FB8[];
extern char D_80138FC8[];
extern const unsigned char D_80152AE8[144];
extern char D_80151EC8[];
extern const unsigned char D_80151E38[144];
Obj *func_800953C0(Obj *obj);
void func_8009D610(Obj *obj, char *text, void *a, void *b);
s32 func_800957C0(Obj *obj, Result *out, s32 a2, void *a3, s32 a4);

/* Original callers supply a dialog receiver; this implementation uses a local one. */
s32 func_80094CE8(void *unused_dialog, char *text) {
    Obj local;
    Result result;
    Obj *obj = &local;
    s32 found;
    s32 value;

    func_800953C0(obj);
    obj->vtable = D_80152AE8;
    local.field_68 = -1;
    local.field_6C = D_80151EC8;
    func_8009D610(obj, text, D_80138FB8, D_80138FC8);
    found = func_800957C0(obj, &result, 1, 0, 0) == 1;
    if (!found) {
        obj->vtable = D_80151E38;
        return 0;
    }
    value = result.value;
    obj->vtable = D_80151E38;
    return value;
}
