#include "common.h"

/* Prefix view: this dispatcher only accesses the queued event's next link. */
typedef struct Event { struct Event *next; } Event;
typedef struct { char pad[0x48]; s32 x48; s32 x4C; s32 x50; char pad54[6]; short x5A; char pad5C[0x18];
    s32 x74; s32 x78; Event *head7C; Event *tail80; s32 x84; } S;
void func_8012EEA4(S *p, s32 msg, void *arg);
s32 func_8012E5F8(S *p, s32 msg, Event *arg) {
    switch (msg) {
    case 3:
        if (p->tail80 != 0) p->tail80->next = arg;
        else p->head7C = arg;
        p->tail80 = arg;
        break;
    case 4:
        p->x78 = 1;
        p->x84 = 0;
        p->x5A = 1;
        p->x74 = 0;
        p->x4C = 0;
        p->x50 = 1;
        p->x48 = 0;
        func_8012EEA4(p, msg, arg);
        break;
    case 9:
        p->x84 = 1;
        break;
    default:
        func_8012EEA4(p, msg, arg);
        break;
    }
    return 0;
}
