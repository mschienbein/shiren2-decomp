#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern void func_8005E134(u16, s32 *, s32 *);
extern u16 func_80083844(void);
s32 func_8005DFE8(u8 *str){ s32 total = 0; u8 *s = str;
  for (;;) { u16 c = *s++; s32 w, h;
    if (c == 0) break;
    if ((c & 0xF0) == 0xF0) { c = *s++ + (c << 8); }
    else if (!(c & 0x80)) continue;
    func_8005E134(c, &w, &h);
    total += w + func_80083844();
  }
  return total; }
