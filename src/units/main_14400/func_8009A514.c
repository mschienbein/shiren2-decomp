#include "common.h"

typedef unsigned char u8;

typedef struct MenuBase { char pad0[0x4C]; const void *vtbl; } MenuBase;
/* Five consecutive objects have this complete 0x40C-byte extent. */
typedef struct MenuPanel {
    MenuBase base;
    char pad50[0xC8 - 0x50];
    MenuBase second;
    char pad118[0x128 - 0x118];
    char unk128[0x194 - 0x128];
    const void *unk194;
    char pad198[0x2A0 - 0x198];
    const void *unk2A0;
    char pad2A4[0x2CC - 0x2A4];
    s32 field_2CC, field_2D0;
    char pad2D4[0x40C - 0x2D4];
} MenuPanel;
typedef MenuPanel Menu8009A514;

/* D_80141A28 (0x70 bytes): menu base with its vtable at +0x4C and an embedded 0x14-byte text
 * object at +0x5C (own vtable at +0, text handle at +0x10); same definition as func_8009A3C4. */
typedef struct {
    const void *vtbl;
    char unknown4[0xC];
    s32 textHandle10;
} Text8009A3C4;

typedef struct {
    char pad0[0x4C];
    const void *vtbl;
    char pad50[0x5C - 0x50];
    Text8009A3C4 unk5C;
} Head8009A3C4;

/* Base part shared by the widgets below (see func_800953C0): vtable at 0x4C. */
typedef struct ObjA8009A514 {
    unsigned char pad_00[0x4C];
    const void *vtable_4C;
    unsigned char pad_50[0x78 - 0x50];
} ObjA8009A514;
typedef struct ObjB8009A514 {
    unsigned char pad_00[0x4C];
    const void *vtable_4C;
    unsigned char pad_50[0x60 - 0x50];
    s32 index_60;
    void *data_64;
    unsigned char pad_68[0x94 - 0x68];
} ObjB8009A514;

/* Global menu objects constructed here. The last three are owned by this file (zero-filled
 * .data, as the original toolchain emitted for objects with constructors); defining them
 * lets gas fill the jal delay slots with their %lo address halves as in the original. */
extern Menu8009A514 D_801404E0;
extern Menu8009A514 D_801408EC;
Menu8009A514 D_80140CF8 = { { 0 } };
Menu8009A514 D_80141104 = { { 0 } };
Menu8009A514 D_80141510 = { { 0 } };
extern ObjA8009A514 D_8014191C;
extern ObjB8009A514 D_80141994;
extern Head8009A3C4 D_80141A28;

/* Initialized original vtables/tables, not BSS. Their full types are unresolved. */
extern u8 D_801528D0[];
extern u8 D_80152CB8[];
extern u8 D_80151EC8[];
extern u8 D_80152620[];
extern const unsigned char D_80151DF8[24];

void *func_80097AB0(void *obj);
void *func_800953C0(void *o);

/* Static constructor routine (listed in the .data constructor tables). */
void func_8009A514(void)
{
    func_80097AB0(&D_801404E0);
    func_80097AB0(&D_801408EC);
    func_80097AB0(&D_80140CF8);
    func_80097AB0(&D_80141104);
    func_80097AB0(&D_80141510);

    func_800953C0(&D_8014191C);
    D_8014191C.vtable_4C = D_801528D0;

    func_800953C0(&D_80141994);
    D_80141994.vtable_4C = D_80152CB8;
    D_80141994.index_60 = -1;
    D_80141994.data_64 = D_80151EC8;

    func_800953C0(&D_80141A28);
    D_80141A28.vtbl = D_80152620;
    D_80141A28.unk5C.vtbl = D_80151DF8;
    D_80141A28.unk5C.textHandle10 = -1;
}
