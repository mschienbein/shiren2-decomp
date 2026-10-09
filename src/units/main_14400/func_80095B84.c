#include "common.h"

typedef struct { s32 x, y; } Point;
typedef struct {
    unsigned char unk00[0x80];
    short unk80;
    void (*unk84)(void *, Point *);
    short unk88;
    void *(*unk8C)(void *, Point);
} VTable;
typedef struct {
    unsigned char unk00[0x20];
    s32 unk20;
    s32 unk24;
    unsigned char unk28[0x24];
    VTable *unk4C;
} Object;
extern s32 func_800957B4(Object *);
extern void func_80048794(Object *);

/* +0x8C tokens can be item pointers or encoded numeric selections. */
void func_80095B84(Object *object, void *wanted) {
    Point point;
    Point copy;
    Point *cursor;
    VTable *table;
    s32 adjustment;
    s32 has_row;
    if (func_800957B4(object) != 0) {
        func_80048794(object);
        return;
    }
    point.y = 0;
    cursor = &point;
    for (;;) {
        has_row = point.y < object->unk24;
        point.x = 0;
        if (has_row) {
            for (;;) {
                if (point.x < object->unk20) {
                    table = object->unk4C;
                    adjustment = table->unk88;
                    copy = point;
                    if (table->unk8C((unsigned char *)object + adjustment, copy) == wanted) {
                        table = object->unk4C;
                        table->unk84((unsigned char *)object + table->unk80, cursor);
                        func_80048794(object);
                        return;
                    }
                    point.x++;
                } else {
                    break;
                }
            }
            point.y++;
        } else {
            break;
        }
    }
    point.y = 0;
    table = object->unk4C;
    table->unk84((unsigned char *)object + table->unk80, &point);
}
