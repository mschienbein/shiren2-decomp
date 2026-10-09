#include "common.h"

typedef unsigned char u8;

/* Embedded 0x18-byte collection; its layout belongs to func_800CEC90. */
typedef struct { u8 storage[0x18]; } Collection800F7228;
/* Partial object view: the collection lives at +0x8C. */
typedef struct {
    u8 pad_00[0x8C];
    Collection800F7228 collection_8C;
} Object800F7228;

extern void func_800E02D0(Object800F7228 *object, void *item);
extern void func_800CE918(Collection800F7228 *list, void *item);

void func_800F7228(Object800F7228 *object, void *item)
{
    func_800E02D0(object, item);
    func_800CE918(&object->collection_8C, item);
}
