#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef struct { u16 buttons; char pad2[0xA]; u16 dir; char pad3[8]; s8 x; s8 y; u8 mag; } Input;
extern void func_80058CE4(u16 *, u16 *, u16 *, u8 *, u8 *);
s32 func_8006E3D0(void){ Input in; s32 ax, ay, m; u16 b;
  func_80058CE4(&in.buttons, 0, 0, (u8 *)&in.x, (u8 *)&in.y);
  ax = in.x < 0 ? -in.x : in.x;
  ay = in.y < 0 ? -in.y : in.y;
  in.mag = ax > ay ? ax : ay;
  in.dir = 0;
  if (in.mag >= 8) {
    if (in.y > 0) in.dir = 0x800; else if (in.y < 0) in.dir = 0x400;
    if (in.x > 0) in.dir |= 0x100; else if (in.x < 0) in.dir |= 0x200;
  }
  if (!(in.buttons & 0x10)) {
    u16 d = in.dir;
    if (d & 0x800) return 0x13;
    if (d & 0x400) return 0x14;
    if (d & 0x200) return 0x15;
    if (d & 0x100) return 0x16;
  }
  b = in.buttons;
  if (b & 0x1000) return 0;
  if (b & 0x8000) return 1;
  if (b & 0x4000) return 2;
  if ((b & 0xA00) == 0xA00) return 0x17;
  if ((b & 0x900) == 0x900) return 0x18;
  if ((b & 0x600) == 0x600) return 0x19;
  if ((b & 0x500) == 0x500) return 0x1A;
  if (!(b & 0x10)) {
    if (b & 0x200) return 0x15;
    if (b & 0x100) return 0x16;
    if (b & 0x800) return 0x13;
    if (in.buttons & 0x400) return 0x14;
  }
  if (in.buttons) return 3;
  return -1; }
