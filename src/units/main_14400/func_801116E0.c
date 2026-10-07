#include "common.h"
typedef unsigned short u16;
extern char *func_80111654(void *obj, s32 mode);
extern char *func_80048480(u16 id);
extern char *func_800ACC90(void *obj);
extern char *func_800ACB40(void *obj);
extern char *func_800ACCDC(void *obj);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
static inline s32 positive(s32 value) { return value > 0; }
char *func_801116E0(void *object, char *buffer, s32 type) { char *value = func_80111654(object, type); if (type < 3 && positive(type)) { char *format = func_80048480(0x243); func_8005EF08(buffer, format, func_800ACC90(object), value); } else { char *arg = func_800ACB40(object); if (arg) { func_8005EF08(buffer, func_80048480(0x2A4), arg, value); } else { char *format = func_80048480(0x243); func_8005EF08(buffer, format, func_800ACCDC(object), value); } } return buffer; }
