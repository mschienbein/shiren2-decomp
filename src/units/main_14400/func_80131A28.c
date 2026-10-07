#include "common.h"

/*
 * Handler 1 of request table D_80148E44 (request 0x0301): func_80131CC0 calls it
 * with the request message and stores the result in the message (+8) for the requester.
 * Partial message view: +0xC points at the channel byte.
 */
typedef struct { char data[0x68]; } Entry;
typedef struct { char pad[0xC]; unsigned char *xC; } S;
extern char D_801E028C[];
extern Entry D_801E00EC[];
s32 func_8002C964(void *a, Entry *e, s32 idx);
s32 func_80131A28(S *p) {
    s32 idx = *p->xC;
    return func_8002C964(D_801E028C, &D_801E00EC[idx], idx);
}
