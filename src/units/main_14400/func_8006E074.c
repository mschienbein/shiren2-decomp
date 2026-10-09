#include "common.h"
typedef struct Input8006E0A8 Input8006E0A8;
extern s32 D_801A70E0;
/* The initialized callback table points to func_8006E0A8, which consumes input. */
extern s32 (*D_8013D434[])(Input8006E0A8 *input);
/* D_8013D3F0 supplies both input records; this handler uses only the first. */
s32 func_8006E074(Input8006E0A8 *input, void *other) { return D_8013D434[D_801A70E0](input); }
