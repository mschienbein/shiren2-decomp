#include "common.h"
extern char *func_801170BC(char *destination, s32 textId);
/* Widget vtable slot 11 (+0x58 this-adjust, +0x5C method): writes the text of item
 * `index` (signed text id bytes at +0x54) into destination; no caller uses a result. */
void func_8009F5E4(signed char *arg, s32 index, char *destination) { arg += index; func_801170BC(destination, arg[0x54]); }
