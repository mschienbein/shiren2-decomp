#include "common.h"

typedef struct { s32 count, field_4, field_8, field_C; } Descriptor;
typedef struct { s32 x, y, kind; } Settings;
/* Spinner widget: label/text strings at +0x50/+0x54 (drawn as text by func_80046F48). */
typedef struct { char pad0[0x50]; char *label_50; char *text_54; s32 field_58, field_5C, field_60, field_64, field_68; } Widget;
extern unsigned char *func_8006A810(unsigned char *dst, s32 value, s32 count);
extern void func_8009543C(Widget *obj, Descriptor *desc);

/* The eighth argument is dereferenced at offsets 0, 4 and 8 in the original. */
void func_8009775C(Widget *obj, char *label, char *text, s32 c, s32 d, s32 e, s32 f, Settings *settings) {
    Descriptor copy;
    Descriptor desc;
    func_8006A810((unsigned char *)&desc, 0, sizeof(desc));
    desc.count = 1;
    desc.field_4 = settings->kind;
    desc.field_8 = settings->x;
    desc.field_C = settings->y;
    copy = desc;
    func_8009543C(obj, &copy);
    obj->label_50 = label;
    obj->text_54 = text;
    obj->field_58 = f;
    obj->field_68 = 0;
    obj->field_60 = c;
    obj->field_5C = d;
    if (e < 9) {
        obj->field_64 = e;
        if (e <= 0) {
            obj->field_64 = 1;
        }
    } else {
        obj->field_64 = 8;
    }
}
