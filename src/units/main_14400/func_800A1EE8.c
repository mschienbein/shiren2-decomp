#include "common.h"
typedef struct { s32 field_00, field_04, field_08; } Object;
typedef struct { unsigned char field_00[0xBB]; signed char field_BB[12]; } Slots;
typedef struct { unsigned char field_00[0x4C]; const void *field_4C; unsigned char field_50[0x10]; } Local;
typedef struct { unsigned char field_00[0x98]; short field_98; void *(*field_9C)(void *); } VTable;
typedef struct { unsigned char field_00[0x24]; VTable *field_24; } Owner;
extern Owner *D_801476B8;
extern char D_8013905C[], D_80152FD8[];
extern const unsigned char D_80151E38[144];
extern Slots *func_800C9E00(void);
extern Local *func_800953C0(Local *);
extern void func_8009F540(Local *, void *);
extern s32 func_800957C0(Local *, s32 *, s32, void *, s32), func_800CD278(void *), func_800AC1FC(void);
static inline void init_local(Local *local) { func_800953C0(local); local->field_4C = D_80152FD8; }
static inline s32 finish(Local *local, s32 result) { local->field_4C = D_80151E38; return result; }
static inline Local *local_address(Local *local) { return local; }
s32 func_800A1EE8(Object *object) {
    Local storage; Local *local; s32 output; s32 i = 0;
    object->field_08 = 0;
    for (;;) { s32 more = i < 12; if (!more) break; if (func_800C9E00()->field_BB[i] != -1) break; i++; }
    if (i >= 12) return -3;
    local = local_address(&storage);
    init_local(local); func_8009F540(local, D_8013905C);
    { s32 failed = func_800957C0(local, &output, 1, 0, 0) != 1; if (failed) return finish(local, -1); }
    if (func_800CD278(D_801476B8->field_24->field_9C((char *)D_801476B8 + D_801476B8->field_24->field_98)) > 0) {
        s32 failed = func_800AC1FC() != 1;
        if (!failed) { object->field_04 = output; object->field_08 = 3; return finish(local, 1); }
    }
    return finish(local, -5);
}
