#include "common.h"

typedef struct { s32 field_0; s32 field_4; s32 field_8; s32 field_C; } Obj801239FC;
extern char D_8015FC54[];
char *func_800AC990(void *obj);
s32 func_8005EF08(char *dst, const char *fmt, ...);

char *func_801239FC(Obj801239FC *obj, char *dest) {
    s32 value = obj->field_C;

    func_8005EF08(dest, D_8015FC54, value, func_800AC990(obj));
    return dest;
}
