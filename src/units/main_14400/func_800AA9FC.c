#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct { u8 pad00[8]; s16 delta08; s16 pad0A; void (*run0C)(void *); } VTable;
/* D_80153B40 and D_8014A7A8 each have eight eight-byte slots and a final zero word. */
typedef struct { VTable prefix; u8 remaining[0x34]; } WholeVTable;
typedef struct { u8 kind; u8 pad01[3]; s32 field04; const VTable *vtable; } Object;


Object *D_80142B10 = 0;
extern const WholeVTable D_80153B40;
extern const WholeVTable D_8014A7A8;
extern s32 D_801F5CD4;
extern void *func_800B6E70(s32 size);
/* The allocator/deallocator interface carries size/object even though these
 * particular arena implementations ignore their arguments.  The original
 * caller loads each argument explicitly. */
extern void func_800B6E80(void *object);
extern Object *func_800C22F0(Object *object, u8 kind, s8 first, s8 second);
extern Object *func_800BA6A0(Object *object, u8 kind, s8 x, s8 y, s32 alternate, s32 argument);
extern Object *func_800C2350(Object *object, u8 kind, s8 x, s8 y, s8 first, s8 second, s32 argument);
extern void func_800C2440(Object *object, u8 kind, s8 first, s8 second);
extern s32 func_80046240(void);
extern void func_800AA944(void);

static inline s8 extra_mode(void)
{
    return D_80142F24.result;
}

void func_800AA9FC(s32 argument)
{
    u8 originalKind = D_80142F18.mode & 0x1F;
    s32 kind = originalKind;
    s32 alternate;
    if (D_80142B10 != 0)
        func_800B6E80(D_80142B10);
    alternate = 0;
    switch (D_80142F18.mode & 0xE0) {
    default:
        kind = 1;
    case 0x60:
        D_80142B10 = func_800C22F0(func_800B6E70(0x10), kind, -1, -1);
        break;
    case 0xA0:
        alternate = 1;
    case 0x20: {
        s8 *coordinates = D_80142F18.coordinates;
        D_80142B10 = func_800BA6A0(func_800B6E70(0x9CC), kind, coordinates[0], coordinates[1], alternate, argument);
        break;
    }
    case 0x40:
        D_80142B10 = func_800C2350(func_800B6E70(0x14), kind, D_80142F18.coordinates[0], D_80142F18.coordinates[1], -1, extra_mode(), argument);
        break;
    case 0x80: {
        Object *object = func_800B6E70(0xC);
        object->vtable = &D_80153B40.prefix;
        object->kind = originalKind;
        object->field04 = 0;
        object->vtable = &D_8014A7A8.prefix;
        func_800C2440(object, kind, -1, -1);
        D_80142B10 = object;
        break;
    }
    }
    D_80142B10->vtable->run0C((u8 *)D_80142B10 + D_80142B10->vtable->delta08);
    D_801F5CD4 = D_80142F18.mode == 0x69;
    if ((func_80046240() ^ 1) != 0)
        func_800AA944();
}
