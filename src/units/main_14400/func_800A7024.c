#include "common.h"

typedef struct {
    unsigned char unk00[0xA];
    unsigned char unk0A;
} Object;
extern s32 func_801F258C(s32, s32);

s32 func_800A7024(Object *object) {
    return func_801F258C(object->unk0A, 1);
}
