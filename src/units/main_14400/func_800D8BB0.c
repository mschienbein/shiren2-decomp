#include "common.h"
typedef unsigned char u8;
extern void *func_800AC5B4(s32 kind, s32 count);
extern void *func_801172E0(void *base);
extern void *func_80117490(void *base);
extern void *func_80117710(void *base);
extern void *func_80117820(void *base);
extern void *func_80117AA0(void *base);
extern void *func_80117D60(void *base);
extern void *func_80117E70(void *base);
extern void *func_80117F10(void *base);
extern void *func_80118020(void *base);
extern void *func_80118130(void *base);
extern void *func_80118390(void *base);
extern void *func_801184A0(void *base);
extern void *func_801185C0(void *base);
extern void *func_801186E0(void *base);
extern void *func_801187D0(void *base);
extern void *func_801188D0(void *base);
extern void *func_80118BD0(void *base);
extern void *func_80118C80(void *base);
extern void *func_80118DB0(void *base);
extern void *func_80118EB0(void *base);
extern void *func_80119610(void *base);
extern void *func_801198E0(void *base);

/* object: unused; supplied by func_8011DBB0 and func_8011DD9C. */
void *func_800D8BB0(void *object, s32 value) {
    u8 kind = value;
    switch (kind) {
    case 1:
        return func_801172E0(func_800AC5B4(0xC, 1));
    case 2:
        return func_80117490(func_800AC5B4(0xC, 1));
    case 3:
        return func_80117710(func_800AC5B4(0xC, 1));
    case 4:
        return func_80117820(func_800AC5B4(0xC, 1));
    case 5:
        return func_80117AA0(func_800AC5B4(0xC, 1));
    case 6:
        return func_80117D60(func_800AC5B4(0xC, 1));
    case 7:
        return func_80117E70(func_800AC5B4(0xC, 1));
    case 8:
        return func_80117F10(func_800AC5B4(0xC, 1));
    case 9:
        return func_80118020(func_800AC5B4(0xC, 1));
    case 10:
        return func_80118130(func_800AC5B4(0xC, 1));
    case 11:
        return func_80118390(func_800AC5B4(0xC, 1));
    case 12:
        return func_801184A0(func_800AC5B4(0xC, 1));
    case 13:
        return func_801185C0(func_800AC5B4(0xC, 1));
    case 14:
        return func_801186E0(func_800AC5B4(0xC, 1));
    case 15:
        return func_801187D0(func_800AC5B4(0xC, 1));
    case 16:
        return func_801188D0(func_800AC5B4(0xC, 1));
    case 17:
        return func_80118BD0(func_800AC5B4(0xC, 1));
    case 18:
        return func_80118C80(func_800AC5B4(0xC, 1));
    case 19:
        return func_80118DB0(func_800AC5B4(0xC, 1));
    case 20:
        return func_80118EB0(func_800AC5B4(0xC, 1));
    case 21:
        return func_80119610(func_800AC5B4(0xC, 1));
    case 22:
        return func_801198E0(func_800AC5B4(0xC, 1));
    }
    return 0;
}
