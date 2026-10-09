#include "common.h"

typedef struct { s32 field_00, field_04; } Pair;
typedef struct { s32 field_00; unsigned char field_04; } Item;
/* Item-set slot +0x3C (delta +0x38): void *(void *self, u32 index). In mode 0x20 field_2F4 is the
 * unit member whose vtable D_801544C0 binds func_800CFC2C(self, u32 index) there (holder argument
 * at 0x800997BC). */
typedef struct { unsigned char fields_00[0x38]; short field_38; void *(*field_3C)(void *self, u32 index); } ItemTable;
typedef struct { void *field_00; ItemTable *field_04; } Owner;
typedef struct { unsigned char fields_00[0x60]; short field_60; s32 (*field_64)(void *, s32); } VTable;
/* Text window subobject (func_800486A4/func_80048650 view): handle at +0xC. */
typedef struct { unsigned char fields_00[0xC]; s32 handle_0C; } Window;
typedef struct { unsigned char fields_00[0x20]; s32 field_20, field_24; Pair field_28; unsigned char fields_30[8]; s32 field_38; unsigned char fields_3C[0x10]; VTable *field_4C; unsigned char fields_50[0x2A4]; Owner *field_2F4; Owner *field_2F8; s32 field_2FC; unsigned char fields_300[0xB4]; s32 field_3B4[8]; s32 field_3D4[4]; s32 field_3E4; Window field_3E8; } State;
typedef struct { s32 field_00, field_04; Pair field_08; } Style;
extern s32 D_80138E08;
extern void *D_801476B8;
extern s32 func_800EB820(void *);
extern s32 func_800EB8B0(s32);
extern unsigned char *func_8006A810(void *, s32, s32);
extern void func_800954E0(State *, Style *);
extern void func_80048870(State *, s32);
extern void *func_8009811C(State *, s32);
extern void func_800487EC(State *, s32, s32, void *);
extern s32 func_80098E34(State *, s32);
extern void func_80047098(State *, void *, s32, s32);
extern void func_80098910(State *);
extern void func_80048650(void *);
extern void func_800487C4(void *);
void func_800982DC(State *state) {
    unsigned char text[0x68];
    Style style, temporary;
    s32 mode, i;
    if (state->field_2FC == 0x100) mode = -1;
    else if (state->field_2FC != 0x400) { mode = 0; if (state->field_2FC != 0x80) mode = func_800EB8B0(func_800EB820(D_801476B8)); }
    else mode = 0;
    if (state->field_2FC == 1 || state->field_2FC == 0x20 || state->field_2FC == 0x8000) {
        i = 0;
        do { s32 next = i + 1; if (state->field_38 < state->field_3D4[next]) break; i = next; } while (i < 3);
        if (i == 1 && state->field_2F4) {
            if (!state->field_3E4) {
                unsigned char kind = 1;
                Owner *owner = state->field_2F4;
                ItemTable *table = owner->field_04;
                Item *item = table->field_3C((unsigned char *)owner + table->field_38, 0);
                if (item) kind = item->field_04;
                func_8006A810(&temporary, 0, 0x10);
                temporary.field_00 = kind; temporary.field_04 = D_80138E08; temporary.field_08 = state->field_28;
                style = temporary;
                func_800954E0(state, &style);
            }
            state->field_3E4 = 1;
        } else {
            if (state->field_3E4) {
                func_8006A810(&temporary, 0, 0x10);
                temporary.field_00 = 10; temporary.field_04 = D_80138E08; temporary.field_08 = state->field_28;
                style = temporary;
                func_800954E0(state, &style);
            }
            state->field_3E4 = 0;
        }
    }
    i = 0;
    for (;;) {
        s32 more = i < 3;
        if (!more) break;
        if (state->field_38 == state->field_3D4[i] && state->field_3B4[i + 1] == state->field_3B4[i] && state->field_3D4[i + 1] != state->field_38) {
            func_80048870(state, 0x78000000);
            func_800487EC(state, 0, 1, func_8009811C(state, i));
            break;
        }
        i++;
    }
    if (i >= 3) {
        i = 0;
        for (;;) {
            s32 index;
            if (i >= state->field_20) break;
            index = state->field_38 * state->field_20 + i;
            { s32 row = func_80098E34(state, index);
            if (row >= 0) {
                VTable *table = state->field_4C;
                func_80048870(state, table->field_64((unsigned char *)state + table->field_60, row));
                func_80047098(state, text, index, mode);
                func_800487EC(state, i, 1, text);
            }
            }
            i++;
        }
    }
    func_80098910(state);
    if (state->field_2FC == 0x20 || state->field_2FC == 0x400) { func_80048650(&state->field_3E8); func_800487C4(&state->field_3E8); }
}
