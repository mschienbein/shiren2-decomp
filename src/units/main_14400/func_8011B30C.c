#include "common.h"
typedef unsigned char u8;
typedef struct { s32 type_00; u8 pad_04[0x17]; u8 kind_1B; } Msg;
extern s32 func_801131F8(void *obj, Msg *msg);
extern char *func_800AC990(void *obj);
extern char *func_800ACC30(u8 kind);
extern void func_800498E4(s32 id, ...);
extern void func_8011AF88(void *obj, u8 kind);
s32 func_8011B30C(void *obj, Msg *msg) {
    u8 kind;
    char *name;
    s32 result;
    switch (msg->type_00) {
    default:
        result = func_801131F8(obj, msg);
        break;
    case 0x20:
        kind = msg->kind_1B;
        name = func_800AC990(obj);
        func_800498E4(0xDF, name, func_800ACC30(kind));
        func_8011AF88(obj, kind);
        result = 1;
        break;
    }
    return result;
}
