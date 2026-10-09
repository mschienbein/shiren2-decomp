#include "common.h"

typedef struct { unsigned char unknown00[0x20]; short adjust20; short unknown22; void (*method24)(void *, void *); } VTable;
typedef struct { unsigned char unknown00[0x24]; VTable *field24; } Object;
extern Object *D_801476B8;
extern char D_80143094[], D_801541CC[];
extern void func_800CA4A4(void *, void *);
extern void func_800A9520(void *), func_800AEDAC(void *), func_800AEF44(void *);
extern void func_800B0054(void *, void *);
extern void func_800A8D04(void *), func_801F29F4(void *), func_800B6224(void *), func_800EF7AC(void *), func_800B7768(void *);
extern s32 func_800C9810(void);
extern void func_80046294(void *), func_80045FF0(void *), func_800CA2C0(void *);
void func_800C9C00(void *object) {
    VTable *table;
    func_800CA4A4(object, D_801541CC);
    func_800A9520(object);
    func_800AEDAC(object);
    func_800AEF44(object);
    func_800B0054(D_80143094, object);
    table = D_801476B8->field24;
    table->method24((char *)D_801476B8 + table->adjust20, object);
    func_800A8D04(object);
    func_801F29F4(object);
    func_800B6224(object);
    func_800EF7AC(object);
    func_800B7768(object);
    if (func_800C9810()) { func_80046294(object); func_80045FF0(object); }
    func_800CA2C0(object);
}
