#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
/* Player table D_80159008 slot 0x9C targets func_800EE07C. */
typedef struct { u8 pad0[0x98]; s16 adjust98; s16 pad9A; u8 *(*inventory9C)(u8 *); } Methods;
typedef struct { u8 pad0[0x24]; const Methods *methods24; } Obj;
typedef struct Item Item;
typedef struct { u8 pad0[0x18]; void *element18; } S;
extern Obj *D_801476B8;
extern s32 func_800CD090(void *container, void *element);
extern void func_800EC0F4(Obj *obj, Item *item);
extern void func_800CE718(S *s, void *obj);
void func_800CFF4C(S *self, Item *item) {
    Obj *player = D_801476B8;
    void *inventory = player->methods24->inventory9C((u8 *)player + player->methods24->adjust98);
    if (func_800CD090(inventory, self->element18) != -1) func_800EC0F4(D_801476B8, item);
    func_800CE718(self, item);
}
