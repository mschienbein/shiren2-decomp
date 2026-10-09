#include "common.h"
typedef struct Unit Unit;
extern s32 func_80049CB4(s32, ...);
extern s32 func_800F069C(void *);
extern char *func_800A3B20(Unit *);
extern void func_800497F0(s32, ...);
void func_800FDBBC(Unit *unit) { s32 event = func_80049CB4(0x5C, unit); s32 message; if (func_800F069C(unit)) message = 0x12E; else message = 0x12F; func_800497F0(message, event, func_800A3B20(unit)); }
