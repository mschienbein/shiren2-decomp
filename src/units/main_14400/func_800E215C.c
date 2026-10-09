#include "common.h"

typedef struct {
    unsigned char unk00[9];
    unsigned char unk09;
} Object;
typedef struct {
    unsigned char unk00[3];
    unsigned char unk03;
} Entry;
extern void *func_800B4D80(Object *);

Entry *func_800E215C(Object *object) {
    Entry *entry = func_800B4D80(object);
    if (entry != 0 && entry->unk03 == (object->unk09 & 0xF)) {
        return entry;
    }
    return 0;
}
