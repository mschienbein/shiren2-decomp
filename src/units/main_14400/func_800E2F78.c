#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x90]; short delta_90; short index_92; s32 (*action_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *vtable_24; } Object;
/* Slot 0x94 includes func_800EA38C. */
s32 func_800E2F78(void *self) { VTable *v = ((Object *)self)->vtable_24; return v->action_94((char *)self + v->delta_90, 0, 5, 0xFE, 0); }
