#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;

typedef struct { s16 delta; s16 pad; void *(*fn)(void *self); } VEntry;
typedef struct { VEntry e[20]; } VTable;
typedef struct { u8 pad[0x24]; VTable *vt; } Player;
typedef struct { s16 delta; s16 pad; void *(*fn)(void *self, u32 index); } ItemEntry;
typedef struct { ItemEntry e[8]; } ItemVTable;
typedef struct { s32 unk0; ItemVTable *vt; } Holder;
typedef struct { u8 kind; u8 sub; } Item;
extern Player *D_801476B8;
/* Menu-system bytes +4 (signed display mode) and +5 (flags). */
extern s8 D_80140160[];
static inline s32 isDisplayModeOne(const s8 *menu) { return menu[4] == 1; }
/* Pool container at .data 0x80147F90 (pool-record pointer +0, method table +4, signed mode
 * byte +8; func_800D4D8C reads the mode with lb 8(a0)). The former label D_80147F98 is this
 * +8 mode byte, not a separate object. */
typedef struct Pool { void *records; void *methods; s8 mode; } Pool;
extern Pool D_80147F90;
extern u8 D_801404E0[];
extern u8 D_80138D90[];
Holder *func_800EBA54(Player *);
s32 func_800EC630(Player *, Item *);
void func_80097B90(void *obj, void *owner, void *holder, s32 mode, void *desc, s32 flag);
void *func_80096748(void) {
    Player *player = D_801476B8;
    u8 kind = 0;
    Item *item = 0;
    Holder *holder;
    s32 mode;
    s32 flag;

    holder = func_800EBA54(player);
    if (holder != 0) {
        item = (Item *)holder->vt->e[7].fn((u8 *)holder + holder->vt->e[7].delta, 0);
        if (item != 0) kind = item->kind;
    }
    switch (kind) {
    case 0x10:
        if (func_800EC630(player, item)) break;
    case 0xF:
    case 0x13:
    case 0x14:
        if (item->sub != 0xF2) holder = 0;
        break;
    }
    flag = 0;
    if (isDisplayModeOne(D_80140160)) {
        mode = 0x8000;
    } else if ((u8)D_80140160[5] & 1) {
        mode = 1;
        if (D_80147F90.mode >= 0) flag = 1;
    } else {
        mode = 0x800;
        flag = 1;
    }
    func_80097B90(D_801404E0, player->vt->e[19].fn((u8 *)player + player->vt->e[19].delta), holder, mode, D_80138D90, flag);
    return D_801404E0;
}
