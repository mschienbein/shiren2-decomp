#include "common.h"
#include "item_records.h"

char *func_800AC990(void *obj);
char *func_80083C90(char *dst, char *src);
char *func_8010E328(Item *obj, char *dst){func_80083C90(dst, func_800AC990(obj)); return dst;}
