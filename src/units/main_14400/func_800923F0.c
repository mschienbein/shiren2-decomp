#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00[0x48];
    short field_48;
    short field_4A;
    u8 (*field_4C)(void *);
} Methods;
typedef struct {
    u8 field_00[0xC];
    Methods *field_0C;
} Object;
typedef struct {
    u8 field_00[0x4C];
    const void *field_4C;
    u8 field_50[0x10];
    s32 field_60;
    void *field_64;
    u8 field_68[0x10];
} Dialog;
extern s32 D_80152130[];
extern s32 D_80151EC8[];
extern s32 D_80152098[];
extern const unsigned char D_80151E38[144];
extern u8 D_80142920[];
extern u8 D_80140118[];
extern Dialog *func_800953C0(Dialog *);
extern s32 func_800A1630(void *, u8);
extern void *func_800AC3CC(u8, void *);
extern void func_80096CE4(Dialog *, void *, void *);
extern void func_80041434(s32);
extern s32 func_800957C0(Dialog *, void *, s32, void *, s32);

static inline void set_methods(Dialog *dialog, const void *methods) {
    dialog->field_4C = methods;
}

s32 func_800923F0(Object *object) {
    u8 output[0x20];
    Dialog dialog;
    u8 metadata[0x30];
    u8 *value;
    func_800953C0(&dialog);
    set_methods(&dialog, D_80152130);
    dialog.field_60 = -1;
    dialog.field_64 = D_80151EC8;
    set_methods(&dialog, D_80152098);
    if (func_800A1630(D_80142920,
        object->field_0C->field_4C((u8 *)object + object->field_0C->field_48))) {
        value = func_800AC3CC(object->field_0C->field_4C((u8 *)object + object->field_0C->field_48), metadata);
        value[0xD] = 0;
        func_80096CE4(&dialog, value, D_80140118);
        func_80041434(1);
        func_800957C0(&dialog, output, 1, 0, 0);
        func_80041434(0);
    }
    set_methods(&dialog, D_80151E38);
    return -1;
}
