#include "common.h"

char *func_800AC990(void *obj);
char *func_80083C90(char *dst, char *src);
char *func_8010E328(void *obj, char *dst){func_80083C90(dst, func_800AC990(obj)); return dst;}
