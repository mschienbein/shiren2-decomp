#include "common.h"

typedef struct {
    unsigned char unk00[0x4C];
    const void *unk4C;
    unsigned char unk50[0xBC];
    s32 unk10C;
    const void *unk110;
} Object;
extern const unsigned char D_80151EC8[]; /* Complete callback table, not a scalar. */
extern const unsigned char D_80153078[152];
extern Object *func_800953C0(Object *);

Object *func_8009FD7C(Object *object) {
    func_800953C0(object);
    object->unk4C = D_80153078;
    object->unk10C = -1;
    object->unk110 = &D_80151EC8;
    return object;
}
