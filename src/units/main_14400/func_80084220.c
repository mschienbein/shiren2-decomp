#include "common.h"
typedef unsigned char u8;
typedef u32 size_t;
typedef struct { u8 text[64]; } Line;
typedef struct { s32 x0; short x4; char pad6[0xE]; s32 x14; char pad18[0xC]; s32 x24; char pad28[4]; s32 x2C; char pad30[0x44]; } Msg;
extern s32 func_80084CD4(void (*handler)(void *));
extern void func_800843D8(void *);
extern s32 func_800827BC(void);
extern void func_80053D50(s32, char *);
extern s32 func_80053C80(char *);
extern size_t func_80032D70(const char *s);
extern s32 func_80053CC8(const char *text, s32 lines);
extern void func_800265E0(void *, s32);
extern u8 *func_80084144(u8 *str, u8 *out, s32 *found);
extern s32 D_8013E908, D_801C3390;
extern Line D_801BF2D0[];
extern u8 D_801C3290[];
extern Msg D_801BA380[];
Msg *func_80084220(char *str, s32 arg1){
  s32 id; s32 count; s32 lines; s32 i; char *line; s32 len; u8 *p; s32 found; Msg *msg;
  id = func_80084CD4(func_800843D8);
  count = 0;
  func_80053D50(func_800827BC(), str);
  lines = func_80053C80(str);
  lines++;
  line = str;
  for (i = 1; i <= lines; i++) {
    if (lines == 1) {
      len = func_80032D70(line) - 1;
    } else {
      len = func_80053CC8(line, 1);
      line[len] = 0;
    }
    p = (u8 *)line;
    do {
      func_800265E0(&D_801BF2D0[D_8013E908 + count], sizeof(Line));
      p = func_80084144(p, D_801BF2D0[D_8013E908 + count].text, &found);
      D_801C3290[D_8013E908 + count] = found;
      count++;
    } while (*p != 0);
    line += len + 1;
  }
  msg = &D_801BA380[id];
  msg->x24 = count;
  D_801C3390 = 1;
  msg->x2C = arg1;
  msg->x14 = D_8013E908;
  msg->x4 = 2;
  D_8013E908 += count;
  return msg;
}
