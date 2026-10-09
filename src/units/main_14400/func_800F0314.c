#include "common.h"
typedef struct { unsigned char pad0[0x9C]; unsigned char field_9C; } Obj800F0314;
extern s32 D_80143094[]; /* Address-only view of the 0x10-byte pool. */
unsigned char *func_800AFD78(void *table, unsigned char id);
s32 func_800AC670(unsigned char *p);
unsigned char *func_800F0314(Obj800F0314 *obj) {
    unsigned char *p;
    s32 bad;
    p = func_800AFD78(&D_80143094, obj->field_9C);
    bad = 0;
    if (p == 0) bad = 1;
    else if (func_800AC670(p) != 0) bad = 1;
    else if (p[1] != 0xAC) bad = 1;
    return bad ? 0 : p;
}
