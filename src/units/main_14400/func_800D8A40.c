#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u16 field_0; u8 pad_2[0x16]; u32 field_18; } State;
extern State *func_800C9E10(void);
extern u8 D_80147600[];
extern char D_80148240[5];
extern u8 func_800C57A0(void *);
extern void func_800C5CF8(void *, s32);
extern u32 func_800C598C(void *);
extern char *func_80048480(u16);
static inline u32 next_code(void) { return func_800C598C(D_80147600) & 0xFFF; }
/* Build the 4-character floor code: reseed the RNG from the floor state, discard 0x63 values,
 * split the 20-bit code into four 5-bit digits and map them through text 0x4DC. */
void func_800D8A40(void) {
    u8 digits[4];
    u32 seed = func_800C9E10()->field_18;
    u16 floor = func_800C9E10()->field_0;
    u32 random = (u8)func_800C57A0(D_80147600);
    u32 code;
    char *text;
    func_800C5CF8(D_80147600, seed ^ (floor << 16) ^ (random << 24));
    { s32 i = 0x62;
      do { --i; func_800C598C(D_80147600); } while (i != -1); }
    code = (random << 12) | next_code();
    { s32 i;
      for (i = 0; i < 4; i++) digits[i] = (code >> (15 - i * 5)) & 31; }
    { char *out = D_80148240; s32 i = 4;
      do { *out++ = 0; --i; } while (i != -1); }
    text = func_80048480(0x4DC);
    { s32 i; for (i = 0; i < 4; i++) D_80148240[i] = text[digits[i]]; }
}
