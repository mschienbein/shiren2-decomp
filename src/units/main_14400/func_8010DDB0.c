#include "common.h"
#include "item_records.h"

extern char *func_800AC990(void *arg0);
extern char *func_80083C90(char *arg0, char *arg1);

char *func_8010DDB0(Item *arg0, char *arg1) {
    func_80083C90(arg1, func_800AC990(arg0));
    return arg1;
}
