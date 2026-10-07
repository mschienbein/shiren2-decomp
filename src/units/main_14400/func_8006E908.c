#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct {
    u8 active;
    u8 field_1;
    u8 pad2[2];
    s32 count1;
    void *buf1a;
    void *buf1b;
    s32 count2;
    void *buf2a;
    void *buf2b;
    s32 field_1C;
    s32 count3;
    void *buf3;
} Pool8006E908;
extern char D_8014C820[];
extern char D_8014C830[];
extern char D_8014C840[];
extern char D_8014C850[];
extern char D_8014C860[];
void *func_8006A8D8(char *name, u32 size);
s32 func_8006E908(void *pool, s32 count1, s32 count2, s32 count3) {
    ((Pool8006E908 *)pool)->active = 0;
    if (count1 != 0) {
        ((Pool8006E908 *)pool)->buf1a = func_8006A8D8(D_8014C820, count1 * 8);
        ((Pool8006E908 *)pool)->buf1b = func_8006A8D8(D_8014C830, count1 * 8);
        if (((Pool8006E908 *)pool)->buf1a == 0 || ((Pool8006E908 *)pool)->buf1b == 0) {
            return -1;
        }
    }
    if (count2 != 0) {
        ((Pool8006E908 *)pool)->buf2a = func_8006A8D8(D_8014C840, count2 * 8);
        ((Pool8006E908 *)pool)->buf2b = func_8006A8D8(D_8014C850, count2 * 8);
        if (((Pool8006E908 *)pool)->buf2a == 0 || ((Pool8006E908 *)pool)->buf2b == 0) {
            return -1;
        }
    }
    ((Pool8006E908 *)pool)->buf3 = func_8006A8D8(D_8014C860, count3 * 4);
    if (((Pool8006E908 *)pool)->buf3 == 0) {
        return -1;
    }
    ((Pool8006E908 *)pool)->active = 1;
    ((Pool8006E908 *)pool)->field_1 = 0;
    ((Pool8006E908 *)pool)->count1 = count1;
    ((Pool8006E908 *)pool)->count2 = count2;
    ((Pool8006E908 *)pool)->count3 = count3;
    ((Pool8006E908 *)pool)->field_1C = 0;
    return 0;
}
