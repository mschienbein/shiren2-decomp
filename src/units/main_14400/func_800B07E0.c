#include "common.h"
typedef unsigned short u16;
/* Pool header iterated over (func_800AF890 layout; count read at +8 by func_800B0808). */
typedef struct Collection Collection;
/* 8-byte pool iterator: collection pointer at +0, halfword cursor at +4. */
typedef struct { Collection *field0; u16 field4; u16 pad6; } Object;
Object *func_800B07E0(Object *obj, Collection *collection) { obj->field0 = collection; obj->field4 = 0; return obj; }
