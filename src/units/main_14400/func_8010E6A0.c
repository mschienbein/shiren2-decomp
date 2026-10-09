#include "common.h"

typedef unsigned short u16;
extern char *func_80048480(u16 id);
extern u16 func_800AE710(void *object);
extern char *func_800AC990(void *object);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);

char *func_8010E6A0(void *object, char *buffer)
{
    char *format = func_80048480(0x240);
    s32 value = func_800AE710(object);
    func_8005EF08(buffer, format, value, func_800AC990(object));
    return buffer;
}
