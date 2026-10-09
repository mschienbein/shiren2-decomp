#include "common.h"

typedef unsigned char u8;

typedef struct Queue Queue;

typedef struct {
    u8 pad00[8];
    void **entries;
} Table80090DC8;

typedef struct {
    u8 pad00[0x74];
    Table80090DC8 *table;
} Source80090DC8;

typedef struct {
    u8 pad000[0x1A0];
    s32 index1A0;
    u8 value1A4;
    u8 pad1A5[0x1B4 - 0x1A5];
    Queue *queue1B4;
} Obj80090DC8;

extern s32 func_8008D4B8(Queue *q, void *val, s32 key);

void func_80090DC8(Obj80090DC8 *obj, Source80090DC8 *src, float value)
{
    obj->value1A4 = value;
    func_8008D4B8(obj->queue1B4, src->table->entries[obj->index1A0], 0);
}
