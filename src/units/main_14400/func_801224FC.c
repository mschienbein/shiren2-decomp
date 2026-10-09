#include "common.h"

typedef struct { signed char value[3]; } Triple;
extern Triple D_80148780[];
extern void func_800D5160(void);
extern void func_801224E8(Triple *);

void func_801224FC(void) {
    s32 index;
    func_800D5160();
    for (index = 2; index > 0; index--) {
        D_80148780[index] = D_80148780[index - 1];
    }
    func_801224E8(D_80148780);
}
