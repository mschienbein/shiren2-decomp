#include "common.h"

extern char *func_800AC990(void *obj);
extern char *func_80083C90(char *dst, char *src);

char *func_80123AEC(void *object, char *output) {
    func_80083C90(output, func_800AC990(object));
    return output;
}
