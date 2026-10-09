#include "common.h"
typedef unsigned char u8;
typedef struct { u32 word0, word1; } Gfx;
typedef struct { float left, top, right, bottom; } Rect;
/* Whole 0x20-byte transition controller (same layout as func_8005D8F8). */
typedef struct {
    short x0, y0, x1, y1;
    unsigned short x2, y2;
    u8 phase, mode;
    u8 level_c, target_c, level_b, target_b, level_a, target_a;
    u8 flags, reserved15, period, tick;
    u8 first, second, selection, reserved1B[5];
} RampState;
extern RampState D_80165960;
extern Gfx D_010000D0[];
extern void func_8005D0DC(Rect *rect);

/* gDPFillRectangle word layout (G_FILLRECT = 0xF6); coordinates are encoded
 * through unsigned int conversions as in the libultra macros. */
#define FILL_W0(lrx, lry) (0xF6000000 | (((u32)(lrx) & 0x3FF) << 14) | (((u32)(lry) & 0x3FF) << 2))
#define FILL_W1(ulx, uly) ((((u32)(ulx) & 0x3FF) << 14) | (((u32)(uly) & 0x3FF) << 2))

/* Masks the screen outside the transition window with fill rectangles and
 * reports whether a first record is selected (0xFF = none). */
unsigned char func_8005D24C(void **display_list)
{
    RampState *state = &D_80165960;
    Rect rect;
    Rect *r = &rect;
    Gfx *gfx = *display_list;
    func_8005D0DC(r);
    { Gfx *command = gfx++; command->word0 = 0xDE000000; command->word1 = (u32)D_010000D0; }
    { Gfx *command = gfx++; command->word0 = 0xFA000000; command->word1 = 0xFF; }
    { Gfx *command = gfx++; command->word0 = 0xE7000000; command->word1 = 0; }
    if (rect.left >= r->right || r->top >= r->bottom) {
        Gfx *command = gfx++; command->word0 = FILL_W0(319, 239); command->word1 = FILL_W1(0, 0);
        *display_list = gfx;
        return 0;
    }
    if (rect.left < 0.0f) rect.left = 0.0f;
    if (r->top < 0.0f) r->top = 0.0f;
    if (r->right > 320.0f) r->right = 320.0f;
    if (r->bottom > 240.0f) r->bottom = 240.0f;
    if (rect.left != 0.0f) {
        Gfx *command = gfx++;
        command->word0 = FILL_W0(rect.left, 239);
        command->word1 = FILL_W1(0, 0);
    }
    if (r->right != 319.0f) {
        Gfx *command = gfx++;
        command->word0 = FILL_W0(319, 239);
        command->word1 = FILL_W1(r->right, 0);
    }
    if (r->left != r->right && r->top != 0.0f) {
        Gfx *command = gfx++;
        command->word0 = FILL_W0(r->right, r->top + 1.0f);
        command->word1 = FILL_W1(r->left, 0);
    }
    if (r->left != r->right && r->bottom != 239.0f) {
        Gfx *command = gfx++;
        command->word0 = FILL_W0(r->right, 239);
        command->word1 = FILL_W1(r->left, r->bottom);
    }
    *display_list = gfx;
    return state->first != 0xFF;
}
