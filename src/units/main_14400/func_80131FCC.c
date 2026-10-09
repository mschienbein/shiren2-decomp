#include "common.h"
typedef struct OSThread OSThread;
extern OSThread D_801CA990;
extern void func_80032B50(OSThread *thread);
void func_80131FCC(void) { func_80032B50(&D_801CA990); }
