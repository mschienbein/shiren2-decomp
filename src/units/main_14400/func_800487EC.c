#include "common.h"

typedef struct {
    s32 fields_00[3];
    s32 field_0C;
} Object;

extern char D_801613E0[];
extern void func_800826FC(s32);
extern void func_80082854(s32, s32);
extern s32 func_8005EF08(char *, const char *, ...);
extern void func_80082A20(void *);

void func_800487EC(Object *object, s32 column, s32 row, void *text) {
    s32 resource = object->field_0C;
    if (resource >= 0) {
        func_800826FC(resource);
        func_80082854(row * 13 - 6, (column * 16) | 12);
        func_8005EF08(D_801613E0, text);
        func_80082A20(D_801613E0);
    }
}
