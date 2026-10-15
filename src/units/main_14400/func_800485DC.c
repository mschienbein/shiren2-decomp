#include "common.h"

void func_8007D204(void);
/* All three ROM callers pass D_801476B8. This facade ignores the player
 * and snapshots global status through func_8007D204; keep the supplied pointer. */
void func_800485DC(void *unused_player) { func_8007D204(); }
