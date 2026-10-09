#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { u8 pad[0x4C]; const void *vt; u8 pad50[0x18]; s32 x68; void *x6C; u8 pad70[0xA0]; } Dlg;
typedef struct RecordChild RecordChild;
/* Whole 0x30-byte record object at D_80147680 (next object D_801476B0): func_800CB268
 * stores +0xA, func_800CA76C stores +0x20 and the child pointer +0x1C, func_800CB288
 * stores +0x24, func_800CB2D4 +0x28 and func_800CA9A8 clears the word at +0x2C. */
typedef struct RecordObject {
    unsigned char pad0[9];      /* +0x00..+0x08 */
    unsigned char flags9;       /* +0x09 */
    signed char valueA;         /* +0x0A */
    unsigned char padB[0x11];   /* +0x0B..+0x1B */
    RecordChild *child1C;       /* +0x1C */
    unsigned char value20;      /* +0x20 */
    unsigned char pad21[3];
    s32 value24;                /* +0x24 */
    s32 value28;                /* +0x28 */
    unsigned char pad2C[4];     /* +0x2C..+0x2F */
} RecordObject;
extern RecordObject D_80147680;
extern u8 D_80141AD0[]; extern u8 D_80154234[]; extern u16 D_80154238[]; extern u8 D_80138D50[]; extern u8 D_80138D60[];
extern const unsigned char D_80152AE8[144]; extern u8 D_80151EC8[]; extern const unsigned char D_80151E38[144];
extern s8 D_80140160[];
extern void func_8009ADE8(void*, s32, u8*, s32); extern s32 func_800957C0(void *object, void *output, s32 modal, void *history, s32 event); extern u8 *func_8009C17C(void*);
extern s32 func_80094E6C(s32); extern u8 *func_80083F34(u8*, s32, u8*); extern char *func_80048480(u16);
extern s32 func_8005EF08(char*, const char*, ...); extern Dlg *func_800953C0(Dlg*); extern void func_8009D610(Dlg*, char*, void*, void*);
extern s32 func_800CA76C(void*, u8); extern void func_800CA9A8(void*, u8*, s32);
static inline void dlg_set_vt(Dlg *d, const void *vt){ d->vt = vt; }
s32 func_800C9E30(s32 arg){
  /* Menu result-path buffer: func_800957C0 writes one word per level and
   * advances by four at 0x800959D8. The original reserves sp+0x18..0x1F;
   * name starts at sp+0x20. The name-entry child selector is null, so only
   * path[0] is written for that menu and only the root result is consumed. */
  s32 path[2]; u8 name[0x10]; char text[0x80]; Dlg dlg; u8 *str; s32 n;
  func_8009ADE8(D_80141AD0, 4, D_80154234, 0);
  func_800957C0(D_80141AD0, path, 1, 0, 0);
  str = func_8009C17C(D_80141AD0);
  if (*str == 0) str = D_80154234;
  n = func_80094E6C(1);
  if (n >= 3) n = 1;
  {
    char *fmt; s32 failed;
    func_80083F34(str, 9, name)[0] = 0;
    fmt = func_80048480(0x28E);
    func_8005EF08(text, fmt, (char *)name, func_80048480(D_80154238[n]));
    func_800953C0(&dlg); dlg_set_vt(&dlg, D_80152AE8); dlg.x68 = -1; dlg.x6C = D_80151EC8;
    func_8009D610(&dlg, text, D_80138D50, D_80138D60);
    failed = func_800957C0(&dlg, path, 1, 0, 0) != 1;
    if (failed) path[0] = 0;
    if (path[0]) {
      func_800CA76C(&D_80147680, arg);
      func_800CA9A8(&D_80147680, str, n);
      D_80140160[6] = 0;
      dlg_set_vt(&dlg, D_80151E38);
      return 1;
    }
    dlg_set_vt(&dlg, D_80151E38);
    return 0;
  } }
