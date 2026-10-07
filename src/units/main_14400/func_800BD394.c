#include "common.h"

typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect800BD394;
typedef struct { s32 x; s32 y; } Pos800BD394;

void func_800B1B58(Pos800BD394 *pos, unsigned short flags);
void func_800B1BE0(Pos800BD394 *pos, s32 flags);

void func_800BD394(void *self, Rect800BD394 *rect) {
    Pos800BD394 pos;

    for (pos.x = rect->x0 + 1;; pos.x += 2) {
        s32 xEnd = rect->x1 - 1;

        if (xEnd < pos.x) {
            break;
        }
        for (pos.y = rect->y0 + 1;; pos.y += 2) {
            s32 yEnd = rect->y1 - 1;

            if (yEnd < pos.y) {
                break;
            }
            func_800B1B58(&pos, 0x4000);
            func_800B1BE0(&pos, 0x200);
        }
    }
}
