#include "common.h"
typedef struct { unsigned char color[4], state[0x20]; float values[8]; unsigned char active, pad45[3]; } SavedState;
extern SavedState D_80138BF4;
typedef unsigned char u8;
typedef struct { u8 pad_0[0x18]; short delta_18, index_1A; void (*write_1C)(void *, s32, void *); } VTable;
typedef struct { u8 pad_0[0x18]; VTable *vtable_18; } Writer;
extern const char D_8014A96C[];
/* The Eye record is serialized as one 0x48-byte object. */

extern void func_8004633C(void);
extern void func_800CA4A4(void *, void *);
void func_80046294(Writer *writer) { VTable *v; func_8004633C(); func_800CA4A4(writer,(void *)D_8014A96C); v=writer->vtable_18; v->write_1C((char *)writer+v->delta_18,0x48,&D_80138BF4); }
