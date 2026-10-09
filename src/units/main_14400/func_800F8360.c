#include "common.h"
typedef struct Actor Actor;
/* Request built by func_800F8348: method table D_80159BA0 at +0, actor at +4. */
typedef struct { void *vtable_00; Actor *actor_04; } Request;
extern void *func_800F7A1C(Actor *actor);
/* Poll callback bound at D_80159BA0 + 0x0C (0x80159BAC); returns the action bundle or null. */
void *func_800F8360(Request *request) { return func_800F7A1C(request->actor_04); }
