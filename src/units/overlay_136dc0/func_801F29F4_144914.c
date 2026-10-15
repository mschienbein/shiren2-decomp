#include "func_801F29F4_144914/types.h"

/* One joint owner: text 801F29F4..801F2ABC, tag 801F638C..801F6395. */
static const char event_flags_tag[] = "EventFlg";

static inline void stream_write(Stream *stream, s32 size, void *buffer) {
    VtEntry *vt = stream->vptr;

    vt[3].pfn((u8 *)stream + vt[3].delta, size, buffer);
}

void func_801F29F4_144914(Stream *stream) {
    func_800CA4A4(stream, event_flags_tag);
    if (D_801F5D0E_147C2E == 0 && D_801F5D10_147C30 != 0) {
        stream_write(stream, 2, &D_801F5D10_147C30);
    } else {
        stream_write(stream, 2, &D_801F5D0E_147C2E);
    }
}

void func_801F2A70_144990(Stream *stream) {
    VtEntry *vt;

    func_800CA4E8(stream, event_flags_tag);
    vt = stream->vptr;
    vt[5].pfn((u8 *)stream + vt[5].delta, 2, &D_801F5D10_147C30);
}
