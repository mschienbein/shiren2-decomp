#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x0; u8 name[4]; u8 b8; u8 b9; u8 padA[2]; } Entry;
/* func_8009EEF0 initializes 30 records before the subobject at 0x1C4. */
typedef struct { u8 pad[0x5C]; Entry entries[30]; } Base;
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);
s32 func_8005EF30(char *dst, const char *fmt, ...);
char *func_80083D04(char *dst, char *src);
char *func_80083F34(void *, s32, char *);
void func_8009EFE8(Base *base, s32 index, char *win) {
    char buf[16];
    char name[16];
    Entry *e = &base->entries[index];
    func_80083C90(win, func_80048480(0x52D));
    func_8005EF30(buf, func_80048480(0x527), index + 1);
    func_80083D04(win, buf);
    func_8005EF30(buf, func_80048480(0x528), e->x0);
    func_80083D04(win, buf);
    *func_80083F34(e->name, 4, name) = 0;
    func_8005EF30(buf, func_80048480(0x529), name);
    func_80083D04(win, buf);
    if (e->b9) {
        func_80083D04(win, func_80048480(0x52B));
    } else if (e->b8) {
        func_8005EF30(buf, func_80048480(0x52A), e->b8);
        func_80083D04(win, buf);
    }
}
