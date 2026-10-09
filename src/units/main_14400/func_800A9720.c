#include "common.h"
typedef unsigned char u8;
typedef struct { u8 count; u8 kind; u8 pad[14]; } Info;
extern char D_801C51B0[];
char *func_80048480(unsigned short id);
void func_800ABB50(Info *info, u8 id, u8 arg);
s32 func_800327C0(char *dst, const char *fmt, ...);
char *func_800A9720(u8 kind, u8 variant, u8 flags) {
    Info info;
    char *name;
    unsigned short id;
    if (kind > 20) return func_80048480(0x3E81);
    switch (kind) {
        case 9:
        case 10:
        case 20:
            return func_80048480(kind + 0x4651);
    }
    func_800ABB50(&info, kind, variant);
    if (info.count == 0) return func_80048480(info.kind + 0x3E81);
    id = ((flags & 1) && (u8)(kind - 1) < 3) ? kind + 0x522 : kind + 0x4651;
    name = func_80048480(id);
    if (flags & 2) {
        func_800327C0(D_801C51B0, func_80048480(0x522), name, info.count);
        return D_801C51B0;
    }
    return name;
}
