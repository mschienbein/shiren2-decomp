#include "common.h"

typedef struct Ent Ent;
typedef struct Item Item;

s32 func_800A2A5C(Ent *ent, Item *item);
void func_800ACD34(Ent *obj);

s32 func_800A2A24(Ent *ent, Item *item) {
    if (func_800A2A5C(ent, item) != 0) {
        func_800ACD34(ent);
        return 1;
    }
    return 0;
}
