#include "common.h"

typedef struct {
    char data[0xA0];
} Obj_801014AC;

extern void *func_800A38FC(s32 size);
extern Obj_801014AC *func_80101460(Obj_801014AC *obj, unsigned char kind);

/* Factory-table entry: construct in the supplied storage, or in a fresh 0xA0-byte block. */
Obj_801014AC *func_801014AC(unsigned char kind, Obj_801014AC *storage) {
    if (storage != 0) {
        return func_80101460(storage, kind);
    }
    return func_80101460(func_800A38FC(sizeof(Obj_801014AC)), kind);
}
