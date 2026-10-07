#include "common.h"
typedef struct {
    s32 pos;
    s32 end;
    char pad8[0xC];
    void *x14;
    char pad18[8];
    s32 base;
} Stream;
extern char D_8014A8D8[];
extern unsigned char D_80138B20[];
void func_80044434(Stream *s);
void *func_80032D94(void *dst, const void *src, u32 count);
void func_800444A4(Stream *s, s32 n, unsigned char *dst) {
    s32 off;
    u32 chunk;
    if (s->end - s->pos < n) {
        s->x14 = D_8014A8D8;
        return;
    }
    for (;;) {
        if (n == 0) return;
        func_80044434(s);
        off = s->pos - s->base;
        chunk = 32 - off;
        if (n < chunk) chunk = n;
        func_80032D94(dst, D_80138B20 + off, chunk);
        n -= chunk;
        s->pos += chunk;
        dst += chunk;
    }
}
