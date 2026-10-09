#include "common.h"

typedef struct Slot Slot;
extern Slot *func_800A88D8(void);

/* Allocation hook: every caller supplies the object size (0x80, 0xAC, 0xE0,
 * ...), but the fixed-record pool behind func_800A88D8 ignores it (a0 is
 * neither read here nor live into func_800A88D8). */
void *func_800A38FC(s32 size)
{
    return func_800A88D8();
}
