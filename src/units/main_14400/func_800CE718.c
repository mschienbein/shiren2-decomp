#include "common.h"

typedef unsigned char u8;

typedef struct { void *key; s32 unk4; u8 *buf; u8 padC[2]; u8 count; } S;
void *func_800AFB80(void *obj);
s32 func_800AFD08(void *table, void *obj);
u8 func_800AFFD0(void *src_table, void *obj, void *dst_table);
void func_800CE718(S *s, void *obj) {
    void *table = func_800AFB80(obj);
    u8 c;
    if (table != s->key) c = func_800AFFD0(table, obj, s->key); else c = func_800AFD08(table, obj);
    if (c != 0xFF) s->buf[s->count++] = c;
}
