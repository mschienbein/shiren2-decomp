#include "common.h"

extern char *func_800AC990(void *arg0);
extern char *func_80083C90(char *dst, char *value);

void *func_8011637C(void *arg0, void *dst) {
    func_80083C90(dst, func_800AC990(arg0));
    return dst;
}
