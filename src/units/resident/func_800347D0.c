#include "common.h"

/* libultra libc xldtob.c: _Ldtob, _Ldunscale, _Genld */

typedef double ldouble;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

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

#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

#define BUFF_LEN 0x20

#define _D0 0
#define _D1 1
#define _D2 2
#define _D3 3
#define _DOFF 4
#define _DFRAC ((1 << _DOFF) - 1)
#define _DMASK (0x7fff & ~_DFRAC)
#define _DMAX ((1 << (15 - _DOFF)) - 1)

#define ALIGN(s, align) (((u32)(s) + ((align)-1)) & ~((align)-1))

ldiv_t func_8002B270(long num, long denom); /* ldiv */
void *func_80032D94(void *dst, const void *src, unsigned int n); /* memcpy */

short func_80034C1C(short *pex, ldouble *px);
void func_80034CB4(_Pft *px, char code, char *p, short nsig, short xexp);

static const double pows[] = { 10e0L, 10e1L, 10e3L, 10e7L, 10e15L, 10e31L, 10e63L, 10e127L, 10e255L };

void func_800347D0(_Pft *px, char code) {
    char buff[BUFF_LEN];
    char *p;
    ldouble ldval;
    short err;
    short nsig;
    short xexp;

    p = buff;
    ldval = px->v.ld;

    if (px->prec < 0) {
        px->prec = 6;
    } else if (px->prec == 0 && (code == 'g' || code == 'G')) {
        px->prec = 1;
    }
    err = func_80034C1C(&xexp, &px->v.ld);
    if (err > 0) {
        __builtin_memcpy(px->s, err == 2 ? "NaN" : "Inf", px->n1 = 3);
        return;
    }
    if (err == 0) {
        nsig = 0;
        xexp = 0;
    } else {
        int i;
        int n;

        if (ldval < 0) {
            ldval = -ldval;
        }
        if ((xexp = xexp * 30103 / 100000 - 4) < 0) {
            n = ALIGN(-xexp, 4), xexp = -n;
            for (i = 0; n > 0; n >>= 1, i++) {
                if ((n & 1) != 0) {
                    ldval *= pows[i];
                }
            }
        } else if (xexp > 0) {
            ldouble factor = 1;

            xexp &= ~3;
            for (n = xexp, i = 0; n > 0; n >>= 1, i++) {
                if ((n & 1) != 0) {
                    factor *= pows[i];
                }
            }
            ldval /= factor;
        }
        {
            int gen = px->prec + ((code == 'f') ? xexp + 10 : 6);

            if (gen > 0x13) {
                gen = 0x13;
            }
            *p++ = '0';
            while (gen > 0 && 0 < ldval) {
                int j;
                int lo = ldval;

                if ((gen -= 8) > 0) {
                    ldval = (ldval - lo) * 1e8;
                }
                for (p += 8, j = 8; lo > 0 && --j >= 0;) {
                    ldiv_t qr;

                    qr = func_8002B270(lo, 10);
                    *--p = qr.rem + '0', lo = qr.quot;
                }
                while (--j >= 0) {
                    *--p = '0';
                }
                p += 8;
            }

            gen = p - &buff[1];
            for (p = &buff[1], xexp += 7; *p == '0'; p++) {
                --gen, --xexp;
            }

            nsig = px->prec + ((code == 'f') ? xexp + 1 : ((code == 'e' || code == 'E') ? 1 : 0));
            if (gen < nsig) {
                nsig = gen;
            }
            if (nsig > 0) {
                char drop;
                int n2;

                if (nsig < gen && p[nsig] > '4') {
                    drop = '9';
                } else {
                    drop = '0';
                }

                for (n2 = nsig; p[--n2] == drop;) {
                    nsig--;
                }
                if (drop == '9') {
                    p[n2]++;
                }
                if (n2 < 0) {
                    --p, ++nsig, ++xexp;
                }
            }
        }
    }
    func_80034CB4(px, code, p, nsig, xexp);
}

short func_80034C1C(short *pex, ldouble *px) {
    unsigned short *ps = (unsigned short *)px;
    short xchar = (ps[_D0] & _DMASK) >> _DOFF;

    if (xchar == _DMAX) {
        *pex = 0;
        return (ps[_D0] & _DFRAC) || ps[_D1] || ps[_D2] || ps[_D3] ? 2 : 1;
    } else if (0 < xchar) {
        ps[_D0] = (ps[_D0] & ~_DMASK) | 0x3ff0;
        *pex = xchar - 0x3fe;
        return -1;
    } else if (0 > xchar) {
        return 2;
    } else {
        *pex = 0;
        return 0;
    }
}

void func_80034CB4(_Pft *px, char code, char *p, short nsig, short xexp) {
    const unsigned char point = '.';

    if (nsig <= 0) {
        nsig = 1, p = "0";
    }

    if (code == 'f' || ((code == 'g' || code == 'G') && (-4 <= xexp) && (xexp < px->prec))) {
        xexp++;
        if (code != 'f') {
            if (((px->flags & FLAGS_HASH) == 0) && nsig < px->prec) {
                px->prec = nsig;
            }
            if ((px->prec -= xexp) < 0) {
                px->prec = 0;
            }
        }
        if (xexp <= 0) {
            px->s[px->n1++] = '0';
            if (0 < px->prec || px->flags & FLAGS_HASH) {
                px->s[px->n1++] = point;
            }
            if (px->prec < -xexp) {
                xexp = -px->prec;
            }
            px->nz1 = -xexp;
            px->prec += xexp;
            if (px->prec < nsig) {
                nsig = px->prec;
            }
            func_80032D94(&px->s[px->n1], p, px->n2 = nsig);
            px->nz2 = px->prec - nsig;
        } else if (nsig < xexp) {
            func_80032D94(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = xexp - nsig;
            if (0 < px->prec || px->flags & FLAGS_HASH) {
                px->s[px->n1] = point, ++px->n2;
            }
            px->nz2 = px->prec;
        } else {
            func_80032D94(&px->s[px->n1], p, xexp);
            px->n1 += xexp;
            nsig -= xexp;
            if (0 < px->prec || px->flags & FLAGS_HASH) {
                px->s[px->n1++] = point;
            }
            if (px->prec < nsig) {
                nsig = px->prec;
            }
            func_80032D94(&px->s[px->n1], p + xexp, nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
    } else {
        if (code == 'g' || code == 'G') {
            if (nsig < px->prec) {
                px->prec = nsig;
            }
            if (--px->prec < 0) {
                px->prec = 0;
            }
            code = code == 'g' ? 'e' : 'E';
        }
        px->s[px->n1++] = *p++;
        if (0 < px->prec || px->flags & FLAGS_HASH) {
            px->s[px->n1++] = point;
        }
        if (0 < px->prec) {
            if (px->prec < --nsig) {
                nsig = px->prec;
            }
            func_80032D94(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
        p = &px->s[px->n1];
        *p++ = code;
        if (0 <= xexp) {
            *p++ = '+';
        } else {
            *p++ = '-';
            xexp = -xexp;
        }
        if (100 <= xexp) {
            if (1000 <= xexp) {
                *p++ = xexp / 1000 + '0', xexp %= 1000;
            }
            *p++ = xexp / 100 + '0', xexp %= 100;
        }
        *p++ = xexp / 10 + '0', xexp %= 10;
        *p++ = xexp + '0';
        px->n2 = p - &px->s[px->n1];
    }
    if ((px->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        int n = px->n0 + px->n1 + px->nz1 + px->n2 + px->nz2;

        if (n < px->width) {
            px->nz0 = px->width - n;
        }
    }
}
