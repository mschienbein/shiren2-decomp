#include "common.h"
typedef short s16;
/* Item method table: +0x30 receiver adjustment, +0x34 name formatter that writes into the
 * supplied destination buffer and returns a character pointer (e.g. func_8010B9FC). */
typedef struct Dispatch { char unk0[0x30]; s16 unk30, unk32; char *(*unk34)(void *, char *); } Dispatch;
typedef struct { s32 unk0, unk4; Dispatch *unk8; } State;
extern s32 D_80143040;
/* One 0x80-byte object: four 0x20-byte name buffers used in rotation. */
extern char D_801C54E2[4][0x20];
char *func_800AE674(State *arg0) {
    Dispatch *dispatch = arg0->unk8;
    s32 index = (D_80143040 + 1) & 3;
    D_80143040 = index;
    return dispatch->unk34((char *)arg0 + dispatch->unk30, D_801C54E2[index]);
}
