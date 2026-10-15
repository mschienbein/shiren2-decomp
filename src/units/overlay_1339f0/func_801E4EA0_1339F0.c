#include "func_801E4EA0_1339F0/types.h"

char *func_80083C90(char *dst, char *src);

/* Pak extension names for slots 1..3 and none (custom text encoding), 8 bytes apart in ROM. */
static const char D_801E7EA4_1369F4[4][8] = { "\x11", "\x12", "\x13", "" };

void func_801E4EA0_1339F0(s32 kind, char *dst) {
    const char *src;

    switch (kind) {
    case 0:
        src = D_801E7EA4_1369F4[0];
        break;
    case 1:
        src = D_801E7EA4_1369F4[1];
        break;
    case 2:
        src = D_801E7EA4_1369F4[2];
        break;
    default:
        src = D_801E7EA4_1369F4[3];
        break;
    }
    func_80083C90(dst, (char *)src);
}
