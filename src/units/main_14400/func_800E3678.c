#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad00[0x1E]; u8 flags1E; } Object;
extern s32 func_800E1CC4(Object *, s32);
extern void func_800D739C(void *);
extern void *func_800A65E4(void *, Object *, void *);
extern void func_800E370C(Object *, u8 *);
void func_800E3678(Object *object, Object *attacker)
{
    u8 direction;
    u8 *p;
    s32 protected;
    if (attacker == 0 || attacker == object) {
        func_800D739C(object);
    } else {
        protected = (attacker->flags1E & 0x7C) && func_800E1CC4(attacker, 1);
        if (protected) {
            func_800D739C(object);
        } else {
            p = &direction;
            func_800A65E4(p, object, attacker);
            func_800E370C(object, p);
        }
    }
}
