#include "func_801F360C_14552C/types.h"

extern "C" void func_801F360C_14552C(const Operand *args, Rec16 *out) {
    func_800265E0(out, sizeof(Rec16));
    out->unk00 = args[0].word;
    out->coordinates.bytes.y = args[1].word;
    out->coordinates.bytes.x = args[2].word;
    out->unk04 = args[3].word;
}
