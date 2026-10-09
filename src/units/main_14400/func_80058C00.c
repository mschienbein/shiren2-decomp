#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u16 field_00; u8 x_02, y_03; u16 field_04, pressed_06, repeat_08, accumulated_0A, previous_0C; u8 delay_0E; } Input;
/* Complete twelve-byte raw input record (func_800585B0 clears 0xC bytes; +0xA is
 * read/written by func_80058680 from the same base). */
typedef struct { u16 buttons_00; u8 x_02, y_03; u16 field_04, buttons_06, buttons_08, previous_0A; } Raw;
extern Raw D_80163118;
extern Input D_80163124;
extern u8 func_8006C540(void);
extern void func_80058A4C(void);
void func_80058C00(void) {
    Input *input = &D_80163124;
    Raw *raw = &D_80163118;
    u8 elapsed = func_8006C540();
    u16 buttons, previous;
    u8 x, y;
    func_80058A4C();
    buttons = raw->buttons_08;
    x = raw->x_02;
    y = raw->y_03;
    previous = input->previous_0C;
    input->repeat_08 = 0;
    input->y_03 = y;
    input->field_00 = buttons;
    input->x_02 = x;
    input->accumulated_0A |= buttons;
    input->pressed_06 = buttons & ~previous;
    if (buttons) {
        u8 delay = input->delay_0E;
        if (delay) {
            if (delay < elapsed) input->delay_0E = 0;
            else input->delay_0E = delay - elapsed;
        } else {
            input->repeat_08 = buttons;
            if (buttons != previous) input->delay_0E = 12;
            else input->delay_0E = 4;
        }
    } else input->delay_0E = 0;
    D_80163118.buttons_08 = 0;
    D_80163124.previous_0C = input->field_00;
}
