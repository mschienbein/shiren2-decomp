#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Complete 12-byte registered client (.data 0x80148E60: {0, D_80148E44, id 0x0300, count 0});
 * func_80131BA0 (registered by func_801319A0) reads +4/+8 and writes +0/+0xA. Same record as
 * func_80131BA0. */
typedef struct Client80131BA0 {
    struct Client80131BA0 *next;
    void **entries;
    u16 id;
    u8 count;
} Client80131BA0;
extern Client80131BA0 D_80148E60;
extern void func_80131C40(Client80131BA0 *target);
void func_80131A04(void) { func_80131C40(&D_80148E60); }
