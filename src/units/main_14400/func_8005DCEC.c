#include "common.h"

extern unsigned char D_80165980[];
extern char D_00157B80[];
extern s32 func_8005DCB0(s32);
extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_8005DCEC(s32 id, unsigned char **data, s32 *width, s32 *height) {
    unsigned char *buf;
    s32 idx = func_8005DCB0(id);
    buf = D_80165980;
    func_8006AAF0(buf, (u32)D_00157B80 + idx * 114, 0x88);
    *width = *(unsigned short *)buf >> 8;
    *height = buf[1];
    *data = buf + 2;
}
