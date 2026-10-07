#include "common.h"
typedef struct { char pad[0x14]; s32 unk14; } Obj8012366C;
s32 func_80116CD8(Obj8012366C *, s32);
s32 func_8012366C(Obj8012366C *self, s32 id) {
    if (id == 13) {
        return self->unk14;
    }
    return func_80116CD8(self, id);
}
