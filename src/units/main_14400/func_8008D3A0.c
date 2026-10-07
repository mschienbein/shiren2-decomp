#include "common.h"
/* All ten +8 targets are no-argument no-ops. The +0xC callback is invoked
 * with (record, owner, destination) at 8008CC64 and its s32 result is consumed.
 * The +0x10 cleanup callback receives the record at 8008C7B4/8008C820. */
typedef void (*RecordHook)(void);
typedef s32 (*RecordApply)(void *record, void *owner, void *destination);
typedef void (*RecordCleanup)(void *record);
typedef struct { s32 a; s32 id; RecordHook b; RecordApply c; RecordCleanup d; } Rec;
extern s32 D_801D85CC;
void func_8008D3A0(Rec *r, s32 a, RecordHook b, RecordApply c, RecordCleanup d) {
    s32 *ctr = &D_801D85CC;
    s32 id = *ctr;
    r->a = a;
    r->b = b;
    r->c = c;
    r->id = id;
    *ctr = id + 1;
    r->d = d;
}
