#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair origin, end; } Rect;
typedef struct { Pair dimensions, position; } Layout;
typedef struct Item Item;
typedef struct {
    u8 pad_00[0x58];
    s32 field_58;
    u8 pad_5C[0x20];
    char text_7C[0x80];
    Rect field_FC;
} Obj;
extern Pair D_80138F58;
extern Pair D_80138F60;
extern Item D_80142480[];
extern char *func_80083CC8(char *a, char *b, u32 n);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_80097240(Obj *menu, Item *items, Layout *layout, Pair *sel);

void func_8009D610(void *object, char *text, void *rect, void *position) {
    Layout layout;
    Layout temporary;
    Obj *obj = object;
    obj->field_FC = *(Rect *)rect;
    func_80083CC8(obj->text_7C, text, 0x7F);
    obj->text_7C[0x7F] = 0;
    func_8006A810((u8 *)&temporary, 0, sizeof(temporary));
    temporary.dimensions = D_80138F58;
    temporary.position = *(Pair *)position;
    layout = temporary;
    func_80097240(obj, D_80142480, &layout, &D_80138F60);
    obj->field_58 = 0;
}
