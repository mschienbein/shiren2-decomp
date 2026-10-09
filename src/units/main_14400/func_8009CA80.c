#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 word_0; u8 bytes_4[3]; u8 flags_7; s32 words_8[4]; } Serial;
/* func_800CB288 and func_800CB2D4 store the trailing flags at +0x24/+0x28. */
typedef struct { Serial serial; s32 field_18; void *child_1C; signed char slot_20; s32 flag_24; s32 flag_28; } Obj800CADFC;
typedef struct { s32 slot; Serial serial; s32 field_1C; } SaveEntry;
typedef struct { u16 id; u16 pad2; void *field_4; s32 value; } MenuItem80096140;
typedef struct { s32 columns; s32 width; s32 x; s32 y; } Layout80096140;
typedef struct { s32 count; s32 width; } Selection80096140;
typedef struct { u8 bytes[0x16C]; } Object_8009C440;
typedef struct { u8 bytes[0x5C]; } Menu80096140;
typedef struct { u8 pad0[0x128]; SaveEntry saves[2]; s32 count_168; s32 count_16C; Object_8009C440 menu_170; MenuItem80096140 items_2DC[2]; Menu80096140 menu_2F4; } S;
extern const u8 D_8015488C[8];
extern s32 D_80138F08[2];
extern Layout80096140 D_80138F10;
extern void *func_800CA760(void *p);
extern s32 func_800CA76C(void *, u8);
extern void func_800CADFC(Obj800CADFC *obj);
/* arg1 is the record-array pointer stored at menu+0x50, not an integer. */
extern void func_8009C440(Object_8009C440 *obj, SaveEntry *entries, s32 count, s32 *layout);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_80097240(Menu80096140 *menu, MenuItem80096140 *items, Layout80096140 *layout, Selection80096140 *sel);
void func_8009CA80(S *obj) {
    /* The slot scan owns its index scope; the per-slot record header is
       released at the end of that scope, so the menu selection below reuses
       the same frame bytes. */
    {
        s32 i = 0;
        obj->count_168 = 0;
        obj->count_16C = 0;
        for (;;) {
            Obj800CADFC localHeader;
            /* ODD_C: header names the validated record after func_800CADFC
               normalises it; binding it in the condition (not before the
               loop) keeps the ROM's a0 = &header recomputation for the
               field_18 load instead of GCC folding it to a stack offset. */
            Obj800CADFC *header;
            if (i >= 2) break;
            func_800CA760(&localHeader);
            if (func_800CA76C(&localHeader, i) == 3
                && (func_800CADFC(&localHeader),
                    (header = &localHeader)->serial.flags_7 & D_8015488C[0])) {
                obj->saves[obj->count_168].slot = i;
                obj->saves[obj->count_168].serial = header->serial;
                obj->saves[obj->count_168++].field_1C = header->field_18;
            } else {
                obj->items_2DC[obj->count_16C].id = i + 0x44E;
                obj->items_2DC[obj->count_16C].field_4 = 0;
                obj->items_2DC[obj->count_16C].value = i;
                obj->count_16C++;
            }
            i++;
        }
    }
    func_8009C440(&obj->menu_170, obj->saves, obj->count_168, D_80138F08);
    D_80138F10.columns = obj->count_16C;
    {
        Selection80096140 copy;
        Selection80096140 selection;
        func_8006A810((u8 *)&selection, 0, 8);
        selection.count = obj->count_16C;
        copy = selection;
        func_80097240(&obj->menu_2F4, obj->items_2DC, &D_80138F10, &copy);
    }
}
