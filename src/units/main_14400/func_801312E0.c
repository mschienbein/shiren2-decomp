#include "common.h"
typedef unsigned char u8;
typedef struct Descriptor { struct Descriptor *next; void **entries; unsigned short kind; u8 count; } Descriptor;
extern Descriptor D_80148E30;
extern void func_80131BA0(Descriptor *descriptor);
void func_801312E0(void) { func_80131BA0(&D_80148E30); }
