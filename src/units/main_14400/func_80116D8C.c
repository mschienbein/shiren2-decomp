#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 kind00, id01; } Object;
extern char *func_800ACB40(void *);
extern s32 func_800AD468(u32);
extern char *func_80048480(u16);
extern s32 func_8005EF08(char *, const char *, ...);
extern char *func_800AC990(void *);
extern char *func_80083C90(char *, char *);
char *func_80116D8C(Object *object, char *buffer)
{
    char *name = func_800ACB40(object);
    s32 unknown = func_800AD468(object->id01) ^ 1;
    if (unknown && name != 0) {
        func_8005EF08(buffer, func_80048480(0x2A2), name);
    } else {
        func_80083C90(buffer, func_800AC990(object));
    }
    return buffer;
}
