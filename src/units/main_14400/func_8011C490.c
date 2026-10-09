#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 a; s32 b; } Pos8011C490;


s32 func_800B274C(s32 kind);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 message_id, ...);
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011C490(void *self, Pos8011C490 *src, void *item) {
    Pos8011C490 pos;
    Pos8011C490 *p = &pos;
    s32 id;
    s32 is_mode1;
    p->a = src->a;
    p->b = src->b;
    is_mode1 = (D_80142F18.mode & 0xE0) == 32;
    if (is_mode1) {
        if (func_800B274C(0) != 0) {
            func_80049CB4(0xE0, p);
            func_80049CB4(0xDB);
            id = 0xDB;
        } else {
            func_80049CB4(0x132);
            id = 0x223;
        }
    } else {
        func_80049CB4(0x11D, p);
        id = 0x225;
    }
    func_800498E4(id);
}
