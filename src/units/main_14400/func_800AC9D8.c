#include "common.h"
#include "item_records.h"
extern char *func_800AC990(void *);
extern char *func_80114BCC(Item *, char *, s32);
char *func_800AC9D8(Item *p) { ItemMethods *t = p->field08; if (t->test1C((char *)p + t->delta18, 30)) { return func_800AC990(p); } else { char *value = func_800AE674(p); if (p->field00 == 9) func_80114BCC(p, value, 0); return value; } }
