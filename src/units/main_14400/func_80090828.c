#include "common.h"
typedef unsigned char u8;
typedef struct Rec Rec;
struct Rec { u8 fields_0[0x14]; s32 field_14; void *field_18; void *field_1C; };
typedef struct { u8 pad_0[2]; u8 count; u8 pad_3[5]; Rec **entries; } Context;
extern void *func_80091450(u32 size);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8008D3A0(Rec *record, s32 tag, void (*begin)(void), s32 (*run)(void *, void *, void *), void (*end)(void *));
extern s32 func_8008DF04(void *stream);
extern void func_80091544(void *item);
extern void func_800907C0(void);
extern s32 func_800907C8(void *record, void *owner, void *context);
extern void func_80090820(void *);
s32 func_80090828(void *stream, s32 size, Context *context)
{
    Rec *record;
    s32 result = 0;
    /* ODD_C: group allocation and parse exits before shared cleanup; also shapes scheduling. */
    do {
        record = func_80091450(sizeof(Rec));
        if (record == 0) {
            result = -1;
            break;
        }
        {
            func_8006A810((u8 *)record, 0, 32);
            func_8008D3A0(record, 0x45585449, func_800907C0, func_800907C8, func_80090820);
            record->field_14 = func_8008DF04(stream);
            /* These archive words encode the two runtime address fields. */
            record->field_18 = (void *)func_8008DF04(stream);
            record->field_1C = (void *)func_8008DF04(stream);
            if (size != 12) {
                result = -1;
            } else {
                u8 index = context->count++;
                context->entries[index] = record;
            }
        }
    } while (0);
    if (result != 0 && record != 0) {
        func_80091544(record);
    }
    return result;
}
