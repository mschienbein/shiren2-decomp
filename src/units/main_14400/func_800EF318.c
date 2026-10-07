#include "common.h"
typedef struct { char pad[0x90]; short field_90; s32 (*field_94)(void *, s32, s32, unsigned char, s32); char pad98[0x40]; short field_D8; s32 (*field_DC)(void *, void *); } VTable;
typedef struct { char pad[0x24]; VTable *field_24; } Obj;
extern s32 func_800E4454(Obj *), func_800E2074(Obj *);
extern char *func_800A3B20(Obj *);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...), func_80049BF0(s32);
s32 func_800EF318(Obj *a, void *b) {
    s32 disabled = 0;
    if (func_800E4454(a) || a->field_24->field_94((char *)a + a->field_24->field_90, 2, 9, 0, 0)) disabled = 1;
    if (disabled) return 0;
    { s32 result; s32 failed = func_800E2074(a) != 1;
      if (!failed) result = a->field_24->field_DC((char *)a + a->field_24->field_D8, b);
      else {
        func_80049CB4(0x128, 0x1A6);
        func_80049CB4(0xAA, a);
        func_800498E4(0x1BD, func_800A3B20(a));
        func_80049BF0(0);
        func_80049CB4(0xAB, a);
        result = 1;
      }
      return result;
    }
}
