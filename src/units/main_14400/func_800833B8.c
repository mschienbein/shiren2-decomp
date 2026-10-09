#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Window draw callback (+0x24), two-form contract: the word is generic function-pointer storage
 * (as func_80081C18 receives it) whose form is selected by the context word at +0x28. A null
 * context means the plain form, registered through func_80081D60 (e.g. func_80054258); a
 * non-null context means the contextual form, registered through func_80081C18 with that
 * context (func_800486A4: func_80048688). The original calls the plain form with no argument
 * (0x8008342C) and the contextual form with the context (0x80083440). */
typedef void (*WindowDrawStorage)(void);
typedef void (*WindowDrawPlain)(void);
typedef void (*WindowDrawWithContext)(void *context);
typedef struct {
    u16 active_00; u16 flags_02; u16 style_04; u16 x_06; u16 y_08; u16 width_0A; u16 height_0C; u16 order_0E;
    u16 dirty_10; u8 pad_12[0x12]; WindowDrawStorage draw_24; void *context_28;
} Window800833B8;
extern s32 D_8013E814;
extern s32 D_8013E818;
extern s32 D_8013E8E4;
extern Window800833B8 *D_8013E81C;
extern u16 D_801A9058[];
extern u8 D_801A8BA0[][40];
extern u32 *D_801A9050[2];
extern void func_800826FC(s32 index);
extern void func_800828FC(void);
void func_800833B8(void) {
    s32 index;
    for (index = D_8013E814; index < D_8013E818; index++) {
        Window800833B8 *window;
        func_800826FC(D_801A9058[index]);
        window = D_8013E81C;
        if (window->dirty_10 != 0) {
            window->dirty_10 = 0;
            func_800828FC();
            if (D_8013E81C->context_28 == 0) ((WindowDrawPlain)D_8013E81C->draw_24)();
            else ((WindowDrawWithContext)D_8013E81C->draw_24)(D_8013E81C->context_28);
        } else {
            s32 start_y = window->y_08;
            s32 end_y = start_y + window->height_0C;
            s32 start_x = window->x_06;
            s32 end_x = start_x + window->width_0A;
            s32 id = D_801A9058[index];
            s32 y;
            for (y = start_y; y < end_y; y++) {
                s32 x;
                s32 pixel_offset = (y * 320 + start_x) * sizeof(u32);
                for (x = start_x; x < end_x; x++, pixel_offset += sizeof(u32)) {
                    if ((D_801A8BA0[y][x] & 15) == id) {
                        u32 *dest = (u32 *)((u8 *)D_801A9050[D_8013E8E4] + pixel_offset);
                        u32 *src = (u32 *)((u8 *)D_801A9050[D_8013E8E4 ^ 1] + pixel_offset);
                        s32 line;
                        for (line = 0; line < 8; line++) {
                            *dest = *src;
                            dest += 40;
                            src += 40;
                        }
                    }
                }
            }
        }
    }
}
