#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Item list: pool +0, entry buffer +8, count +0xE. */
typedef struct { ItemPool *key; s32 unk4; u8 *buf; u8 padC[2]; u8 count; } S;
void func_800CE718(S *s, void *obj) {
    ItemPool *table = func_800AFB80(obj);
    u8 c;
    if (table != s->key) c = func_800AFFD0(table, obj, s->key); else c = func_800AFD08(table, obj);
    if (c != 0xFF) s->buf[s->count++] = c;
}
