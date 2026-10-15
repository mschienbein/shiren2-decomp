#include "common.h"
#include "item_records.h"
extern char *func_801170BC(char *, s32);
/* Kind-0x12 constructor func_80117080 installs D_8015DAF8. The text-id word's
 * initialization remains a caller precondition; the constructor does not set it. */
char *func_801170F4(Item *arg0, char *arg1) { func_801170BC(arg1, arg0->payload.value.value0C); return arg1; }
