#include "common.h"

typedef struct ListHolder ListHolder;

extern void func_800A1888(ListHolder *holder, void *list);

ListHolder *func_800A1860(ListHolder *object, void *list) {
    func_800A1888(object, list);
    return object;
}
