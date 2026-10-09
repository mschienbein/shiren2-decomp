#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x40]; short delta_40; short index_42; s32 (*query_44)(void *, void *, u8 *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *vtable_24; } Object;
/* Slot 0x44 includes func_800ECD20, which writes a one-byte result. */
s32 func_800A44F4(void *self, void *target) {
    u8 result;
    VTable *vtable = ((Object *)self)->vtable_24;
    return vtable->query_44((char *)self + vtable->delta_40, target, &result);
}
