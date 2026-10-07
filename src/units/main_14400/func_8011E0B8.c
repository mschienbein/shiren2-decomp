#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { unsigned char field_0[0x58]; short field_58; s32 (*field_5C)(void *, s32 *); } VTable;
typedef struct { Point field_0; unsigned char field_8[0x14]; unsigned short field_1C; unsigned char field_1E[6]; VTable *field_24; } Object;
extern s32 func_800A7D20(Object *);
extern void func_800A59A4(Object *);
extern s32 func_80049CB4(s32, ...);
extern void func_800A6218(Object *, Point *, s32);
static inline void position(Point *out, Object *arg) { out->x = arg->field_0.x; out->y = arg->field_0.y; }
static inline void set_message_value(s32 *message, s32 value) { message[4] = value; }
/* Trap apply slot +0x54 supplies five pointers; self, actor and direction
 * are unused by this override. */
void func_8011E0B8(void *self, void *actor, void *left, void *direction, void *right)
{
    s32 blocked = 0;
    if (func_800A7D20(left) || (((Object *)left)->field_1C & 2)) blocked = 1;
    if (!blocked && right != left) {
        Point a, b;
        s32 message[6];
        position(&a, right);
        position(&b, left);
        func_800A59A4(right);
        func_800A59A4(left);
        ((Object *)right)->field_0 = b;
        ((Object *)left)->field_0 = a;
        func_80049CB4(0x1A, right, left);
        func_800A6218(right, &b, 1);
        func_800A6218(left, &a, 1);
        message[0] = 0x14;
        set_message_value(message, 5);
        ((Object *)right)->field_24->field_5C((unsigned char *)right + ((Object *)right)->field_24->field_58, message);
        ((Object *)left)->field_24->field_5C((unsigned char *)left + ((Object *)left)->field_24->field_58, message);
    }
}
