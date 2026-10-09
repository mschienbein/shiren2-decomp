#include "common.h"
typedef unsigned short u16;
typedef struct Part800E8D0C Part800E8D0C;
/* func_8010EAF4 returns its clamped total full width; this caller narrows it (andi after the call). */
extern u32 func_8010EAF4(Part800E8D0C *part);
u16 func_8011146C(Part800E8D0C *part) { return func_8010EAF4(part); }
