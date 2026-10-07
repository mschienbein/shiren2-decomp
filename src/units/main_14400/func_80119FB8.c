#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 message_id, ...);
void func_80049A04(u16 message_id, ...);
/* Item-effect slot +0x44 supplies self, actor and item; this override ignores all three. */
void func_80119FB8(void *self, void *actor, void *item) {
    func_80049CB4(0x132);
    func_800498E4(0x223);
    func_80049A04(0xC9);
}
