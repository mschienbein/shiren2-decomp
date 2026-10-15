#include "common.h"
#include "item_records.h"
char *func_800AC990(void *);
char *func_80083C90(char *, char *);
char *func_8011726C(Item *a, char *b) {
    func_80083C90(b, func_800AC990(a));
    return b;
}
