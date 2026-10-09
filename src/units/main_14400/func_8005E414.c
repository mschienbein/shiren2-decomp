#include "common.h"

/* libultra-style xprintf.c _Putfld with 32-bit integer conversions and no
 * floating-point conversions; the switch jump table is this file's .rodata. */

typedef u32 size_t;

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

#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

/* local-arithmetic-qualification: GCC 2.8.1 va-mips.h o32 va_arg aligns the
 * ABI argument save area cursor; this is not a game-object address. */
#define va_arg(ap, type) \
    ((ap) = (char *)((((s32)(ap) + 3) & -4) + sizeof(type)), *(type *)(void *)((char *)(ap) - sizeof(type)))

extern size_t func_80032D70(const char *s); /* strlen */
extern void func_8005E260(Pft *px, char code); /* _Litob */

void func_8005E414(Pft *px, void **pap, char code, char *ac) {
    px->n0 = px->nz0 = px->n1 = px->nz1 = px->n2 = px->nz2 = 0;
    switch (code) {
    case 'c':
        ac[px->n0++] = va_arg(*pap, s32);
        break;
    case 'd':
    case 'i':
        px->v.l = va_arg(*pap, s32);
        if (px->qual == 'h') {
            px->v.l = (short)px->v.l;
        }
        if (px->v.l < 0) {
            ac[px->n0++] = '-';
        } else if (px->flags & FLAGS_PLUS) {
            ac[px->n0++] = '+';
        } else if (px->flags & FLAGS_SPACE) {
            ac[px->n0++] = ' ';
        }
        px->s = &ac[px->n0];
        func_8005E260(px, code);
        break;
    case 'x':
    case 'X':
    case 'u':
    case 'o':
        px->v.l = va_arg(*pap, s32);
        if (px->qual == 'h') {
            px->v.l = (unsigned short)px->v.l;
        }
        if ((px->flags & FLAGS_HASH) && px->v.l != 0) {
            ac[px->n0++] = '0';
            if (code == 'x' || code == 'X') {
                ac[px->n0++] = code;
            }
        }
        px->s = &ac[px->n0];
        func_8005E260(px, code);
        break;
    case 'n':
        if (px->qual == 'h') {
            *va_arg(*pap, unsigned short *) = px->nchar;
        } else {
            *va_arg(*pap, u32 *) = px->nchar;
        }
        break;
    case 'p':
        /* %p prints the pointer's numeric value through the integer field. */
        px->v.l = (long)va_arg(*pap, void *);
        px->s = &ac[px->n0];
        func_8005E260(px, 'x');
        break;
    case 's':
        px->s = va_arg(*pap, char *);
        if (px->s != 0) {
            px->n1 = func_80032D70(px->s);
        }
        if (px->prec >= 0 && px->prec < px->n1) {
            px->n1 = px->prec;
        }
        break;
    case '%':
        ac[px->n0++] = '%';
        break;
    default:
        ac[px->n0++] = code;
        break;
    }
}
