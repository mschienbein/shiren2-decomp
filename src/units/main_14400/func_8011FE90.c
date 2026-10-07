#include "common.h"
typedef short s16;

extern short D_80156A1C, D_80156A26;
void func_800EB598(void *obj, s16 amount, s16 alternate_amount);
/* Actor-effect slot +0x44 supplies self and actor; this override ignores self. */
void func_8011FE90(void *a, void *b){func_800EB598(b, D_80156A1C, D_80156A26);}
