#include "common.h"
typedef unsigned char u8;
extern u8 *D_8015380C[];
extern u8 func_800AE98C(u8 *item);
extern char *func_800B0A70(u8 id, s32 force);
/* The selected message lookup returns a character pointer, including NULL. */
char *func_800ACAEC(u8 *item) {
    u8 *ids = D_8015380C[item[0]];
    if (ids != 0) {
        return func_800B0A70(ids[func_800AE98C(item)], 0);
    }
    return 0;
}
