#include "common.h"
typedef unsigned char u8;
typedef struct { u32 start, end; s32 type; } Resource;
extern Resource D_801398C0[20];
extern u32 D_801630B4;
extern u8 *D_801630BC, *D_801630C0;
extern void func_80054FA8(u32 arg0, s32 arg1, void *dst, s32 size, void *header);
extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);
extern void func_8006B4FC(u8 *buf);
extern u8 *func_8006B580(s32 arg0);
extern void func_8006B6F4(u8 *buf);
extern void func_8006CD14(void);
extern void func_8013591C(void *src, void *dst, s32 width, void *work);
extern void func_80135ED4(s32 a);
s32 func_80054DF0(u32 mode) {
    s32 size;
    if (mode >= 20 || !D_801630BC) return -1;
    if (D_801630B4 != mode) {
        D_801630B4 = mode;
        if (mode) {
            func_8006CD14();
            D_801630C0 = 0;
            size = D_801398C0[D_801630B4].end - D_801398C0[D_801630B4].start;
            switch (D_801398C0[D_801630B4].type) {
            case 0:
                func_8006AAF0(D_801630BC, D_801398C0[D_801630B4].start, size);
                break;
            case 1: {
                u8 *work = func_8006B580(1);
                u8 *compressed = work + 0x3840;
                func_8006AAF0(compressed, D_801398C0[D_801630B4].start, size);
                func_80135ED4(0xFF);
                func_8013591C(compressed, D_801630BC, 320, work);
                func_8006B4FC(work);
                func_8006B6F4(work);
                break;
            }
            case 2:
                D_801630C0 = D_801630BC + 0x12C00;
                func_80054FA8(D_801398C0[D_801630B4].start, size, D_801630BC, 0x12C00, D_801630C0);
                break;
            }
        }
    }
    return 0;
}
