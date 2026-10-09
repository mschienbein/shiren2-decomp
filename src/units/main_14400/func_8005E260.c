#include "common.h"

typedef struct {
    union {
        long l;
        double ld;
    } v;
    char *s;
    s32 n0;
    s32 n1;
    s32 n2;
    s32 nz0;
    s32 nz1;
    s32 nz2;
    s32 prec;
    s32 width;
    u32 nchar;
    u32 flags;
    char qual;
} Pft;

#define FLAGS_MINUS 4
#define FLAGS_ZERO 16
#define BUFF_LEN 0x18

extern void *func_80032D94(void *dst, const void *src, u32 count); /* memcpy */

static char D_8013B604[] = "0123456789abcdef";
static char D_8013B618[] = "0123456789ABCDEF";

void func_8005E260(Pft *px, char code) {
    char buff[BUFF_LEN];
    const char *digs;
    s32 base;
    s32 i;
    unsigned long ulval;

    digs = (code == 'X') ? D_8013B618 : D_8013B604;
    base = (code == 'o') ? 8 : ((code != 'x' && code != 'X') ? 10 : 16);
    i = BUFF_LEN;
    ulval = px->v.l;
    if ((code == 'd' || code == 'i') && px->v.l < 0) {
        ulval = -ulval;
    }
    if (ulval != 0 || px->prec != 0) {
        buff[--i] = digs[ulval % base];
    }
    px->v.l = ulval / base;
    while (px->v.l > 0 && i > 0) {
        long v = px->v.l;

        px->v.l = v / base;
        buff[--i] = digs[v % base];
    }
    px->n1 = BUFF_LEN - i;
    func_80032D94(px->s, buff + i, px->n1);
    if (px->n1 < px->prec) {
        px->nz0 = px->prec - px->n1;
    }
    if (px->prec < 0 && (px->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        i = px->width - px->n0 - px->nz0 - px->n1;
        if (i > 0) {
            px->nz0 += i;
        }
    }
}
