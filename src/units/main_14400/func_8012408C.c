#include "common.h"
typedef unsigned char u8;

typedef struct { char pad[0x1E]; unsigned char flags; } Entity;
typedef struct { unsigned char kind; unsigned char id; } Item;
extern s32 func_80049CB4(s32, ...);
extern s32 func_800E9144(Entity *);
extern s32 func_800A7024(Entity *);
extern void func_800E4734(Entity *, void *actor, s32);
extern void func_800498E4(s32, ...);
extern char *func_800AC990(void *object);
extern void func_8010E2CC(Item *);
/* ODD_C: boolean helper; GCC 2.8.1 expands its result as sne (sltu rd,$zero,rs), as the original does. */
static inline u8 differs(u32 value, u32 other) { return value != other; }
/* D_8015FDD8+0x44 (0x8015FE1C): func_80115EB0 supplies seven pointers and consumes
 * the s32 result. self, source_position and direction are caller-supplied but
 * unused here; actor is forwarded to func_800E4734. */
s32 func_8012408C(void *self, void *actor, void *source_position, void *source,
                  void *direction, Entity *e, Item *item) {
    s32 ok;
    s32 give;
    func_80049CB4(0xE9, source);
    if (e != 0) {
        if ((e->flags >> 2) & 1) {
            func_800498E4(func_800E9144(e) ? 0xF4 : 0x224);
        } else {
            ok = (e->flags & 0x7C) && !(e->flags & 0xC) && !func_800A7024(e);
            if (ok) {
                func_800E4734(e, actor, 0xA4);
            } else {
                func_800498E4(0x224);
            }
        }
    }
    if (item != 0) {
        give = item->kind == 8 && differs(item->id, 0xA4);
        if (give) {
            func_800498E4(0xF6, func_800AC990(item));
            func_8010E2CC(item);
        }
    }
    return 1;
}
