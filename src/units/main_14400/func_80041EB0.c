#include "common.h"

/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];
s32 func_80041EB0(void) { return D_80140160[4] == 1; }
