#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* +0x24 is the inherited integer list-count slot, implemented by 800CE710. */
typedef struct { u8 pad_00[0x20]; short adjust_20; short pad_22; s32 (*method_24)(void *); } ListMethods;
/* List object (func_800CE6A0 layout): pool pointer at +0, method table at +4. */
typedef struct { void *field_00; const ListMethods *field_04; u8 pad_08[0x18]; } List;
typedef struct { u8 field_00, field_01; u8 pad_02[0xA]; union { List list; struct { u8 flags, field_0D, field_0E; } bytes; } field_0C; u8 field_2C; } Item;
extern const u16 D_80157F74[], D_80157EBC[], D_80157E14[], D_80157D94[];
extern const u16 D_80157E8C[], D_80157EF4[], D_80157E64[], D_80157E04[];
extern const u16 D_80157F1C[], D_80157DD4[], D_80157E10[], D_80157F70[];
extern const u16 D_80157F38[], D_80157E00[], D_80157DD0[], D_80157F3C[];
extern const u16 D_80157E9C[], D_80157E98[], D_80157D88[];
const u16 *const D_80153908[] = {
 0, D_80157F74, D_80157EBC, D_80157E14, D_80157D94, D_80157E8C,
 D_80157EF4, D_80157E64, D_80157E04, D_80157F1C, D_80157DD4,
 D_80157E10, D_80157F70, D_80157F38, D_80157E00, D_80157DD0,
 D_80157F3C, D_80157E9C, D_80157E98, D_80157D88, 0
};
const u16 D_8015395C[] = {
 0, 0x8F1, 0x8F2, 0x8F0, 0x8F0, 0x8F0, 0x8F3, 0x8F4, 0x8F0,
 0x8F5, 0x8F6, 0x8F0, 0x8F0, 0x8F0, 0x8F0, 0x8F0, 0x8F0,
 0x8F0, 0x8F0, 0x8F0, 0
};
extern char D_801C5562[0x10E];
extern char *func_80048480(u16 id);
extern s32 func_800AD468(u32 bit);
extern u16 func_800EFDFC(u8 a, u8 b);
extern char *func_800AE674(void *item);
extern char *func_80112AB0(Item *item);
extern char *func_80121D80(Item *item, char *buffer);
extern s32 func_800327C0(char *dest, const char *format, ...);
extern u8 func_800AE98C(u8 *item);
static inline s32 unidentified(s32 value) { return value ^ 1; }
static inline s32 list_count(List *list) { return list->field_04->method_24((u8 *)list + list->field_04->adjust_20); }
char *func_800AE7BC(Item *item) {
 s32 identified = 0;
 s32 name;
 if (item->field_00 == 2) identified = item->field_0C.bytes.flags & 1;
 if (identified) name = 0x8F7;
 else if (unidentified(func_800AD468(item->field_01))) name = D_8015395C[item->field_00];
 else if (item->field_01 == 0xF1) name = func_800EFDFC(item->field_0C.bytes.field_0D, item->field_0C.bytes.field_0E);
 else if (item->field_01 == 0xF2) {
  if (item->field_0C.bytes.field_0D >> 7) name = 0x8F8;
  else {
   const char *format = func_80048480(0x233);
   char *text = func_800AE674(item);
   func_800327C0(D_801C5562, format, text);
   return D_801C5562;
  }
 } else {
  if (item->field_00 == 0x11) return func_80112AB0(item);
  if (item->field_01 == 0xAC) {
   s32 occupied = 0;
   if (item->field_2C != 0xFF || list_count(&item->field_0C.list)) occupied = 1;
   if (occupied) {
    const char *format = func_80048480(0x8F9);
    char *text = func_80121D80(item, func_800AE674(item));
    func_800327C0(D_801C5562, format, text);
    return D_801C5562;
   }
  }
  {
   const u16 *names = D_80153908[item->field_00];
   name = names[func_800AE98C((u8 *)item)];
  }
 }
 return func_80048480(name);
}
