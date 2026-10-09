#include "common.h"

/* The widget occupies D_80140470..D_801404DF. */
typedef struct Widget70 { char pad0[0x4C]; const void *field_4C; char pad50[0x20]; } Widget70;
extern Widget70 D_80140470;
extern char D_80152260[];
Widget70 *func_800953C0(Widget70 *obj);

void func_800979E0(void) {
    Widget70 *obj = &D_80140470;

    func_800953C0(obj);
    obj->field_4C = D_80152260;
}
