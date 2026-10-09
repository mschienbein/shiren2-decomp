#include "common.h"
typedef struct { s32 value; void *object; } Handle;
extern Handle *func_800CB330(Handle *handle);
extern s32 func_800CB33C(Handle *handle);
extern void func_800CB4D4(Handle *handle);
s32 func_80045D1C(void) {
    Handle handle;
    s32 value;
    Handle *cursor;
    func_800CB330(&handle);
    func_800CB33C(&handle);
    cursor = &handle;
    value = handle.value;
    func_800CB4D4(cursor);
    func_800CB4D4(cursor);
    return value;
}
