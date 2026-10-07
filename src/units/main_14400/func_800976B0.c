#include "common.h"

/* Numeric spinner widget constructor: vtable at +0x4C. func_8009775C stores
 * label at +0x50 and text at +0x54 (both drawn as text by func_80046F48),
 * stores c/d/e/f as integers and copies the three-word bounds record read
 * through its last argument. */
typedef struct { unsigned char pad[0x4C]; void *vtable; } Widget;
typedef struct { s32 w0; s32 w4; s32 w8; } Bounds;
extern char D_80152260[];
Widget *func_800953C0(Widget *w);
void func_8009775C(Widget *w, char *label, char *text, s32 c, s32 d, s32 e, s32 f, Bounds *bounds);
Widget *func_800976B0(Widget *w, char *label, char *text, s32 c, s32 d, s32 e, s32 f, Bounds *bounds) {
    func_800953C0(w);
    w->vtable = D_80152260;
    func_8009775C(w, label, text, c, d, e, f, bounds);
    return w;
}
