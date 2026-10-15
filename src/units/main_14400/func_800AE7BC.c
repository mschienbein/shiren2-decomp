#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef unsigned short u16;
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
extern char *func_80112AB0(Item *item);
extern char *func_80121D80(Item *item, char *buffer);
extern s32 func_800327C0(char *dest, const char *format, ...);
/* This generic helper inspects the first two object-representation bytes. */
extern u8 func_800AE98C(u8 *identity);
static inline s32 unidentified(s32 value) { return value ^ 1; }
static inline s32 list_count(ItemList *list) { return list->methods->count24((u8 *)list + list->methods->delta20); }
char *func_800AE7BC(Item *item) {
 s32 identified = 0;
 s32 name;
 if (item->field00 == 2) identified = item->payload.consumable.flags0C & 1;
 if (identified) name = 0x8F7;
 else if (unidentified(func_800AD468(item->field01))) name = D_8015395C[item->field00];
 else if (item->field01 == 0xF1) name = func_800EFDFC(item->payload.containerF1.field0D, item->payload.containerF1.field0E);
 else if (item->field01 == 0xF2) {
  if (item->payload.appearance.flags0D >> 7) name = 0x8F8;
  else {
   const char *format = func_80048480(0x233);
   char *text = func_800AE674(item);
   func_800327C0(D_801C5562, format, text);
   return D_801C5562;
  }
 } else {
  if (item->field00 == 0x11) return func_80112AB0(item);
  if (item->field01 == 0xAC) {
   s32 occupied = 0;
   if (item->payload.pot.field2C != 0xFF || list_count(&item->payload.pot.list)) occupied = 1;
   if (occupied) {
    const char *format = func_80048480(0x8F9);
    char *text = func_80121D80(item, func_800AE674(item));
    func_800327C0(D_801C5562, format, text);
    return D_801C5562;
   }
  }
  {
   const u16 *names = D_80153908[item->field00];
   name = names[func_800AE98C((u8 *)item)];
  }
 }
 return func_80048480(name);
}
