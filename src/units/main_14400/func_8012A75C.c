#include "common.h"
extern void *D_801CA6FC;
extern void func_8012BDD0(void *bank, void *sampleBase);
/* Both relocation inputs are live; the retained global is the bank pointer. */
void func_8012A75C(void *bank, void *sampleBase) {
    func_8012BDD0(bank, sampleBase);
    if (D_801CA6FC == 0) D_801CA6FC = bank;
}
