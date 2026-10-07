#include "common.h"

/* libultra libc xlitob.c: _Litob
 * `%` and `/` emit libgcc calls __umoddi3 (func_80036A70) and __udivdi3 (func_80036510). */

typedef double ldouble;

typedef struct {
    long long quot;
    long long rem;
} lldiv_t;

typedef struct {
    union {
        long long ll;
        ldouble ld;
    } v;
    char *s;
    int n0;
    int nz0;
    int n1;
    int nz1;
    int n2;
    int nz2;
    int prec;
    int width;
    unsigned int nchar;
    unsigned int flags;
    char qual;
} _Pft;

#define FLAGS_MINUS 4
#define FLAGS_ZERO 16

#define BUFF_LEN 0x18

lldiv_t func_8002B2F4(long long num, long long denom); /* lldiv */
void *func_80032D94(void *dst, const void *src, unsigned int n); /* memcpy */

static char ldigs[] = "0123456789abcdef";
static char udigs[] = "0123456789ABCDEF";

void func_80035260(_Pft *px, char code) {
    char buff[BUFF_LEN];
    const char *digs;
    int base;
    int i;
    unsigned long long ullval;

    digs = (code == 'X') ? udigs : ldigs;
    base = (code == 'o') ? 8 : ((code != 'x' && code != 'X') ? 10 : 16);
    i = BUFF_LEN;
    ullval = px->v.ll;
    if ((code == 'd' || code == 'i') && px->v.ll < 0) {
        ullval = -ullval;
    }
    if (ullval != 0 || px->prec != 0) {
        buff[--i] = digs[ullval % base];
    }
    px->v.ll = ullval / base;
    while (px->v.ll > 0 && i > 0) {
        lldiv_t qr = func_8002B2F4(px->v.ll, base);

        px->v.ll = qr.quot;
        buff[--i] = digs[qr.rem];
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
