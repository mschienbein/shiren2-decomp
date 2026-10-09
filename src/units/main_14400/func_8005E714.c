#include "common.h"

/* libultra-style xprintf.c: _Printf (func_8005E714), its sprintf output
 * callback (func_8005EC74), vsprintf (func_8005ECA8) and a full-width
 * vsprintf variant (func_8005ECF8). The literal-text scan steps over two-byte
 * glyph codes (lead byte 0xF?) so their trail byte is never taken for '%'. */

typedef u32 size_t;
typedef unsigned short u16;
typedef char *va_list;

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

/* Output callback: consumes n bytes of s and returns the next output cursor
 * (0 on failure). */
typedef char *(*OutFunc)(char *arg, const char *s, u32 n);

#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16
#define MAX_PAD 20

/* local-arithmetic-qualification: GCC 2.8.1 va-mips.h o32 va_arg aligns the
 * ABI argument save area cursor; this is not a game-object address. */
#define va_arg(ap, type) \
    ((ap) = (char *)((((s32)(ap) + 3) & -4) + sizeof(type)), *(type *)(void *)((char *)(ap) - sizeof(type)))

extern const char *func_80032D30(const char *s, s32 c); /* strchr */
extern s32 func_8005E708(u32 value); /* isdigit */
extern void func_8005E414(Pft *px, void **pap, char code, char *ac); /* _Putfld */
extern void *func_80032D94(void *s1, const void *s2, size_t n); /* memcpy */

static char D_8013B62C[] = "                    "; /* spaces */
static char D_8013B644[] = "00000000000000000000"; /* zeroes */

#define PUT(str, n)                                 \
    if (0 < (n)) {                                  \
        if ((arg = (*pfn)(arg, str, n)) != 0) {     \
            x.nchar += (n);                         \
        } else {                                    \
            return -1;                              \
        }                                           \
    }

#define PAD(str, n)                                 \
    if (0 < (n)) {                                  \
        s32 i, j = (n);                             \
        for (; 0 < j; j -= i) {                     \
            i = MAX_PAD < (u32)j ? MAX_PAD : j;     \
            PUT(str, i);                            \
        }                                           \
    }

#define ATOI(i, a)                                  \
    for (i = 0; func_8005E708(*a); a++) {           \
        if (i < 999) {                              \
            i = i * 10 + *a - '0';                  \
        }                                           \
    }

s32 func_8005E714(OutFunc pfn, char *arg, const char *fmt, void *args) {
    Pft x;

    x.nchar = 0;
    while (1) {
        static const char D_8014C3E0[] = { ' ', '+', '-', '#', '0', '\0' };
        static const s32 D_8014C3E8[] = { FLAGS_SPACE, FLAGS_PLUS, FLAGS_MINUS, FLAGS_HASH, FLAGS_ZERO, 0 };
        const char *s;
        const char *t;
        u16 c;
        s32 len;
        char ac[32];

        s = fmt;
        len = 0;
        while (1) {
            c = s[len++];
            if ((c & 0xF0) == 0xF0) {
                c = (c << 8) + s[len++];
            }
            if (c == 0) {
                break;
            }
            s += len;
            len = 0;
            if (c == '%') {
                s--;
                break;
            }
        }
        PUT(fmt, s - fmt);
        if (c == 0) {
            return x.nchar;
        }
        s++;
        for (x.flags = 0; (t = func_80032D30(D_8014C3E0, *s)) != 0; s++) {
            x.flags |= D_8014C3E8[t - D_8014C3E0];
        }
        if (*s == '*') {
            x.width = va_arg(args, s32);
            if (x.width < 0) {
                x.width = -x.width;
                x.flags |= FLAGS_MINUS;
            }
            s++;
        } else {
            ATOI(x.width, s);
        }
        if (*s != '.') {
            x.prec = -1;
        } else if (*++s == '*') {
            x.prec = va_arg(args, s32);
            s++;
        } else {
            ATOI(x.prec, s);
        }
        x.qual = func_80032D30("hlL", *s) ? *s++ : '\0';
        func_8005E414(&x, &args, *s, ac);
        x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;
        if (!(x.flags & FLAGS_MINUS)) {
            PAD(D_8013B62C, x.width);
        }
        PUT(ac, x.n0);
        PAD(D_8013B644, x.nz0);
        PUT(x.s, x.n1);
        PAD(D_8013B644, x.nz1);
        PUT(x.s + x.n1, x.n2);
        PAD(D_8013B644, x.nz2);
        if (x.flags & FLAGS_MINUS) {
            PAD(D_8013B62C, x.width);
        }
        fmt = s + 1;
    }
}

/* _Printf output callback for vsprintf: copies the chunk and returns the next
 * destination byte. */
char *func_8005EC74(char *base, const char *src, u32 count) {
    func_80032D94(base, src, count);
    return base + count;
}

s32 func_8005ECA8(char *buf, const char *fmt, void *args) {
    s32 n = func_8005E714(func_8005EC74, buf, fmt, args);
    if (n >= 0) {
        buf[n] = 0;
    }
    return n;
}

/* Format into a local buffer, then widen ASCII digits, space, '+' and '-'
 * to two-byte glyph codes; '#'/'@' control tags and 0xF? lead-byte pairs are
 * copied through unchanged. */
s32 func_8005ECF8(char *dst, const char *fmt, va_list args) {
    char buf[0x200];
    char map[8] = "\xF0\xCF\xF0\xC1\xF0\xAC\xF0\xAD";
    char *start;
    s32 i;
    char c;

    func_8005ECA8(buf, fmt, args);
    i = 0;
    start = dst;
    while (buf[i] != 0) {
        c = buf[i];
        if (c >= '0' && c <= '9') {
            *dst++ = map[0];
            *dst++ = map[1] + buf[i++] - '0';
        } else {
            if (c == ' ') {
                i++;
                *dst++ = map[2];
                *dst++ = map[3];
            } else if (c == '+') {
                i++;
                *dst++ = map[4];
                *dst++ = map[5];
            } else if (c == '-') {
                i++;
                *dst++ = map[6];
                *dst++ = map[7];
            } else if (c == '\n') {
                *dst++ = buf[i++];
            } else if (c == '#' || c == '@') {
                i++;
                *dst++ = c;
                if (buf[i] == '-') {
                    *dst++ = buf[i++];
                }
                while (buf[i] >= '0' && buf[i] <= '9') {
                    *dst++ = buf[i++];
                }
                if ((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z')) {
                    *dst++ = buf[i++];
                }
            } else {
                i++;
                if ((c & 0xF0) != 0xF0) {
                    *dst++ = c;
                } else {
                    *dst++ = c;
                    *dst++ = buf[i++];
                }
            }
        }
    }
    *dst = 0;
    return dst - start;
}
