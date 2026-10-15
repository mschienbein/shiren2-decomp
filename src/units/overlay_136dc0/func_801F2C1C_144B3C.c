#include "func_801F2C1C_144B3C/types.h"

/* One joint owner: text 801F2C1C..801F2D68, tag 801F639C..801F63A8. */
static const char creature_tag[] = "ES_Creature";

static inline void stream_write(Stream *stream, s32 size, void *buffer) {
    VtEntry *vt = stream->vptr;

    vt[3].pfn((u8 *)stream + vt[3].delta, size, buffer);
}

void func_801F2C1C_144B3C(Obj801F63A8 *obj, Stream *stream) {
    obj->save.packed = obj->field_00 + (obj->field_04 << 2) + (obj->field_10 << 3) + (obj->field_14 << 4)
                     + (obj->field_0C << 6) + (obj->field_08 << 7);
    func_800CA4A4(stream, creature_tag);
    stream_write(stream, sizeof(obj->save), &obj->save);
}

static inline void stream_read(Stream *stream, s32 size, void *buffer) {
    VtEntry *vt = stream->vptr;

    vt[5].pfn((u8 *)stream + vt[5].delta, size, buffer);
}

void func_801F2CB8_144BD8(Obj801F63A8 *obj, Stream *stream) {
    func_800CA4E8(stream, creature_tag);
    stream_read(stream, sizeof(obj->save), &obj->save);
    obj->field_00 = obj->save.packed & 3;
    obj->field_04 = (obj->save.packed >> 2) & 1;
    obj->field_10 = (obj->save.packed >> 3) & 1;
    obj->field_14 = (obj->save.packed >> 4) & 3;
    obj->field_0C = (obj->save.packed >> 6) & 1;
    obj->field_08 = obj->save.packed >> 7;
}
