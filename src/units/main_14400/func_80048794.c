#include "common.h"

typedef struct {
    char pad00[0xC];
    s32 index;
} Obj80048794;

extern void func_800835AC(u32 index);
extern void func_8006E6D8(void);

void func_80048794(Obj80048794 *obj) {
    if (obj->index >= 0) {
        func_800835AC(obj->index);
        func_8006E6D8();
    }
}
