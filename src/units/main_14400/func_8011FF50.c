#include "common.h"
typedef short s16;
extern short D_80156A1E;
extern short D_80156A28;
extern void func_800EB598(void *obj, s16 amount, s16 alternate_amount);
/* Actor-effect slot +0x44 supplies self and actor; this override ignores self. */
void func_8011FF50(void *unused, void *a) { func_800EB598(a, D_80156A1E, D_80156A28); }
