#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef short s16;
typedef struct Ent Ent;
typedef struct { u8 pad_0[0x60]; s16 offset_60; s16 pad_62; s32 (*method_64)(void *, Ent *, s32); } VTable;
typedef struct { void *field_0; VTable *field_4; } Owner;
typedef struct { Owner *field_0; Ent *field_4; } Request;
extern s32 func_800A99D0(void);
extern s32 func_800AE9AC(Ent *, s32, s32);

/* The caller (0x800999EC) passes its menu object as the first argument; this function does not use it. */
s32 func_8009A230(void *unused, Request *request)
{
    if ((func_800A99D0() ^ 1) == 0) {
        if ((D_80142F18.mode ^ 79) != 0) {
            Owner *owner = request->field_0;
            VTable *table = owner->field_4;
            if ((table->method_64((u8 *)owner + table->offset_60, request->field_4, 0) ^ 1) == 0) {
                return func_800AE9AC(request->field_4, 2, 0) < 1;
            }
        }
        return 0;
    }
    return 0;
}
