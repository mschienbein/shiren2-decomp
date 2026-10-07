#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x1E]; u8 flags; } Obj8011CB68;
s32 func_80049CB4(s32 id, ...);
void func_80049A04(u16 message_id, ...);
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011CB68(void *arg0, Obj8011CB68 *obj, void *item) {
    if ((obj->flags >> 2) & 1) {
        func_80049CB4(0x132);
        func_80049A04(0xE0);
    }
}
