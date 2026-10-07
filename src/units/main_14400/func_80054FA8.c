#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

u8 *func_8006B580(s32 arg0);
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_800D8E60(u8 *dst, u8 *buf);
void *func_80032D94(void *dst, const void *src, u32 size);
void func_8006B4FC(u8 *buf);
void func_8006B6F4(u8 *buf);
void func_80054FA8(u32 arg0, s32 arg1, void *dst, s32 size, void *header) {
    u8 *buf = func_8006B580(1);
    u8 *body = buf + (size + 0x200);
    func_8006AAF0(body, arg0, arg1);
    func_800D8E60(body, buf);
    func_80032D94(dst, buf + 0x200, size);
    func_80032D94(header, buf, 0x200);
    func_8006B4FC(buf);
    func_8006B6F4(buf);
}
