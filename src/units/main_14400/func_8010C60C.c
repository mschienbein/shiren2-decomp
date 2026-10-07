#include "common.h"
typedef struct { char s[5]; } Str5;
typedef struct { unsigned char x0; char pad[0xD]; signed char xE; } Unit;
extern unsigned char D_80148450[];
extern Str5 D_8015D044;
extern Str5 D_8015D054;
extern char D_8015D04C[];
extern char D_8015D05C[];
s32 func_8010BBF4(Unit *u, unsigned char *list);
s32 func_800ACEB4(Unit *u);
char *func_80048480(unsigned short id);
char *func_80083D04(char *dst, char *src);
s32 func_800A2910(unsigned char a, unsigned char b, unsigned short out[2]);
char *func_8010C60C(Unit *u, char *out, unsigned char limit) {
    unsigned char *list = D_80148450;
    s32 known = func_8010BBF4(u, list) & 0xFF;
    s32 total = u->xE;
    s32 i;
    if (limit != 0) {
        if (limit < known) known = limit;
        if (limit < total) total = limit;
    }
    if (func_800ACEB4(u) != 2) {
        *(Str5 *)out = D_8015D044;
        for (i = 0; i < total; i++) {
            func_80083D04(out, func_80048480(0x4F8));
        }
    } else {
        *(Str5 *)out = D_8015D054;
        i = 0;
        for (;;) {
            unsigned short msg[2];
            if (i >= known) break;
            func_800A2910(u->x0, list[i], msg);
            i++;
            func_80083D04(out, func_80048480(msg[0]));
        }
        func_80083D04(out, D_8015D05C);
        for (; i < total; i++) {
            func_80083D04(out, func_80048480(0x4F7));
        }
    }
    func_80083D04(out, D_8015D04C);
    return out;
}
