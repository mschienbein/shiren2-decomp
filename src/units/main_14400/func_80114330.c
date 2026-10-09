#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Member Member;
extern char *func_800AE674(void *obj);
extern void func_80049A04(u16 id, ...);
extern void *func_8011422C(u8 *);
extern s32 func_800CD508(Member *member, u8 *item);
extern s32 func_800CD278(Member *member);
s32 func_80114330(u8 *obj, u8 *item) {
    char *name = func_800AE674(obj);
    s32 accepted;
    u16 message;
    switch (item[0]) {
    case 9:
        message = 0x99;
        goto report;
    case 17:
        message = 0x9A;
    report:
        func_80049A04(message);
        return 0;
    }
    accepted = func_800CD508(func_8011422C(obj), item) == 1;
    if (accepted) return 1;
    if (!func_800CD278(func_8011422C(obj))) func_80049A04(0x9B, name);
    else func_80049A04(0x9C, func_800AE674(item), name);
    return 0;
}
