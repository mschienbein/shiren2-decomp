#include "common.h"
char *func_800AC990(void *);
char *func_80083C90(char *, char *);
void *func_8011726C(void *a, void *b) {
    func_80083C90(b, func_800AC990(a));
    return b;
}
