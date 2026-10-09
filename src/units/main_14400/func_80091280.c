#include "common.h"

typedef unsigned char u8;

typedef struct Stream Stream;

/* Resource callbacks registered by func_8008D3A0 (types as in func_800905DC). */
typedef void (*ResInitFn)(void);
typedef s32 (*ResLoadFn)(void *res, void *owner, void *ctx);
typedef void (*ResFreeFn)(void *res);

/* 0x24-byte 'FUNC' resource: registration header, three mode bytes, then the point
 * table read by func_80091198. */
typedef struct {
    s32 tag;
    s32 id;
    ResInitFn init;
    ResLoadFn load;
    ResFreeFn release;
    s32 field_14;
    u8 mode_18;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
    u32 count;
    void *points;
} Function;

/* Owner list: count at +2, record pointer array at +8. */
typedef struct {
    u8 field_00;
    u8 field_01;
    u8 count;
    u8 field_03;
    s32 field_04;
    Function **functions;
} FunctionList;

#define TAG_FUNC 0x46554E43

void *func_80091450(u32 size);
u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_8008D3A0(Function *rec, s32 tag, ResInitFn init, ResLoadFn load, ResFreeFn release);
s32 func_8008DF04(Stream *stream);
u32 func_8008E0C4(Stream *stream, void *dst, u32 len);
s32 func_80091198(Stream *stream, Function *function);
void func_80091544(void *item);
void func_80090BD0(void);
s32 func_80090E80(void *res, void *owner, void *ctx);
void func_80091160(void *res);

/* Reads one function resource whose encoded length must be `expected` bytes and appends
 * it to the list; returns 0, or -1 (freeing the record) on failure. */
s32 func_80091280(Stream *stream, s32 expected, FunctionList *list)
{
    s32 result = 0;
    s32 consumed;
    Function *function;

    /* ODD_C: early-exit group; each failure breaks to the shared free of the record. Also shapes
     * scheduling: the if/else-if chain and goto-done forms each differ in 3 words. */
    do {
        function = func_80091450(sizeof(Function));
        if (function == 0) {
            result = -1;
            break;
        }
        func_8006A810((u8 *)function, 0, sizeof(Function));
        func_8008D3A0(function, TAG_FUNC, func_80090BD0, func_80090E80, func_80091160);
        function->field_14 = func_8008DF04(stream);
        func_8008E0C4(stream, &function->mode_18, 1);
        func_8008E0C4(stream, &function->field_19, 1);
        func_8008E0C4(stream, &function->field_1A, 1);
        function->count = func_8008DF04(stream);
        consumed = func_80091198(stream, function);
        if (consumed < 0) {
            result = -1;
            break;
        }
        if (expected != consumed + 11) {
            result = -1;
            break;
        }
        list->functions[list->count++] = function;
    } while (0);
    if (result && function) func_80091544(function);
    return result;
}
