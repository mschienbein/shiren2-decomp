#include "func_801E5BF4_134744/types.h"

extern Layout801E5A30 D_80138EE0;
extern Selection801E5BF4 D_80138EF0;
void func_80097240(Menu801E6030 *menu, MenuEntry801E5BF4 *items, Layout801E5A30 *layout, Selection801E5BF4 *sel);
char *func_80083F34(void *src, s32 len, char *dst);
char *func_80048480(u16 id);
s32 func_800327C0(char *dst, const char *fmt, ...);
char *func_80083C90(char *dst, char *src);
char *func_80083D04(char *dst, char *src);
void func_8009D610(void *object, char *text, void *rect, void *position);
void func_8009D6E4(void *object, s32 flag);

void *func_801E5BF4_134744(List801E5A30 *list, s32 offset) {
    s32 i;
    s32 n;
    u8 *slots;
    char text[80];
    char line[80];
    char name[16];

    offset /= 4;
    switch (list->mode) {
    case 1:
        slots = D_801E7830_136380;
        n = 0;
        for (i = 0; i < 3; i++) {
            if (slots[i] == 0xFF) {
                list->entries[n] = D_801E7850_1363A0[i + 2];
                n++;
            }
        }
        D_80138EE0.unk0 = n;
        D_80138EF0.count = n;
        func_80097240(&list->menu, list->entries, &D_80138EE0, &D_80138EF0);
        return &list->menu;
    case 2:
        n = 0;
        for (i = 0; i < 2; i++) {
            if (list->flags[i] == 0) {
                list->entries[n] = D_801E7850_1363A0[i];
                n++;
            }
        }
        slots = D_801E7830_136380;
        for (i = 0; i < 3; i++) {
            if (slots[i] == 0xFF) {
                list->entries[n] = D_801E7850_1363A0[i + 2];
                n++;
            }
        }
        D_80138EE0.unk0 = n;
        D_80138EF0.count = n;
        func_80097240(&list->menu, list->entries, &D_80138EE0, &D_80138EF0);
        return &list->menu;
    case 3:
        *func_80083F34(list->items[offset].text, 4, name) = 0;
        func_800327C0(text, func_80048480(0x449), list->items[offset].value + 1, name);
        func_80083C90(line, func_80048480(0x4FE));
        func_80083D04(line, text);
        func_8009D610(list->text_box, line, D_80138EA4, D_80138E9C);
        func_8009D6E4(list->text_box, 0);
        return list->text_box;
    }
    return 0;
}
