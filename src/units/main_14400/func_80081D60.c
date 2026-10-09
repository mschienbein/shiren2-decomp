#include "common.h"
/* Window draw callback contract: func_80081C18 stores the callback as generic function-pointer
 * storage (window +0x24) next to its context (+0x28); func_800833B8 calls the plain form
 * void (*)(void) when the context is null and the contextual form void (*)(void *) otherwise.
 * This wrapper registers a plain callback with a null context (0x80081D6C). */
s32 func_80081C18(s32 a, s32 b, s32 c, s32 d, void (*callback)(void), void *arg);
s32 func_80081D60(s32 a, s32 b, s32 c, s32 d, void (*callback)(void)) { return func_80081C18(a, b, c, d, callback, 0); }
