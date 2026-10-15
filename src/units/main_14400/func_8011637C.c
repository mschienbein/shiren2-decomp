#include "common.h"
#include "item_records.h"

extern char *func_800AC990(void *arg0);
extern char *func_80083C90(char *dst, char *value);

char *func_8011637C(Item *arg0, char *dst) {
    func_80083C90(dst, func_800AC990(arg0));
    return dst;
}
