#include "common.h"
typedef void (*Callback)(void);
/* Callback table at 0x80149FC0: 39 functions followed by a null sentinel
 * at 0x8014A05C. */
extern Callback D_80149FC0[40];

/* Reads the first table entry; integrated at its call site. */
static inline Callback firstCallback(Callback (*table)[40]) {
    return (*table)[0];
}

void func_800D8F38(void) {
    Callback *entry;
    if (firstCallback(&D_80149FC0) != 0) {
        entry = D_80149FC0;
        do {
            Callback callback = *entry++;
            callback();
        } while (*entry != 0);
    }
}
