#include "common.h"
typedef struct S S;
extern void func_800B8628(S *self, unsigned char mode);
extern void func_800B8B34(S *self);
extern void func_800B91AC(S *self);
extern void func_800B95DC(S *self);
extern void func_800B17A4(void);
extern unsigned char D_80143392;
void func_800BFE50(S *self) {
    func_800B8628(self, 0);
    func_800B8B34(self);
    func_800B91AC(self);
    func_800B95DC(self);
    func_800B17A4();
    D_80143392 = 0;
}
