#include "common.h"

/* Message queue: D_80139610 entries of NUL-terminated text packed in D_80139638. */
extern unsigned char D_80139610;
extern unsigned char D_80139638[0x50];

/* Returns entry n of the queue, or an empty string when n is out of range. */
char *func_8004935C(s32 n)
{
    unsigned char *p;

    if (n >= D_80139610) {
        return "";
    }
    p = D_80139638;
    for (n--; n != -1; n--) {
        while (*p++ != 0) {
        }
    }
    return (char *)p;
}
