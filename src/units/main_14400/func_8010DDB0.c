#include "common.h"

extern char *func_800AC990(void *arg0);
extern char *func_80083C90(char *arg0, char *arg1);

char *func_8010DDB0(void *arg0, char *arg1) {
    func_80083C90(arg1, func_800AC990(arg0));
    return arg1;
}
