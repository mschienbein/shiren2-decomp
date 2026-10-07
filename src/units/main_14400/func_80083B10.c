#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 w[6]; } Stream80083B10;
typedef struct { s32 magic; s32 size; s32 lengthOffset; s32 literalOffset; u8 pad10[0x530]; } Header80083B10;
extern char D_8014D378[];
void *func_8006A8D8(char *name, u32 size);
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_80083960(Stream80083B10 *stream, u8 *src, void *table, s32 count);
u32 func_80083AB4(Stream80083B10 *stream);
u8 func_80083A10(Stream80083B10 *stream);
u16 func_80083A60(Stream80083B10 *stream);
void func_8006A9D4(void *address);
s32 func_80083B10(u8 *src, u8 *dst) {
    Stream80083B10 flags;
    Stream80083B10 copies;
    Stream80083B10 literals;
    Header80083B10 *hdr;
    u32 bits = -1;
    s32 count;
    s32 size;
    u8 *end;
    u16 code;
    u32 len;
    u8 *from;

    hdr = func_8006A8D8(D_8014D378, 0x540);
    func_8006AAF0(hdr, (u32)src, 0x10);
    size = hdr->size;
    end = dst + size;
    func_80083960(&copies, src + hdr->lengthOffset, (u8 *)hdr + 0x40, 0x100);
    func_80083960(&literals, src + hdr->literalOffset, (u8 *)hdr + 0x140, 0x400);
    func_80083960(&flags, src + 0x10, hdr, 0x40);
    count = 0;
    do {
        if (count == 0) {
            bits = func_80083AB4(&flags);
            count = 0x20;
        }
        if (bits & 0x80000000) {
            *dst++ = func_80083A10(&literals);
        } else {
            code = func_80083A60(&copies);
            len = code >> 12;
            from = dst - (code & 0xFFF) - 1;
            if (len != 0) {
                len += 2;
            } else {
                len = func_80083A10(&literals) + 0x12;
            }
            do {
                *dst++ = *from++;
            } while (--len != 0);
        }
        bits <<= 1;
        count--;
    } while (dst != end);
    func_8006A9D4(hdr);
    return size;
}
