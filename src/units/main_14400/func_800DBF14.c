#include "common.h"
typedef struct { void *container; void *item; } Link800DBF14;
typedef struct {
    unsigned char pad_00[4]; const void *vtable_04;
    unsigned char pad_08[8]; Link800DBF14 link_10;
} Obj800DBF14;
extern const unsigned char D_80158568[];
extern void *func_800DA904(void *self, s32 kind, unsigned char *params);
extern Link800DBF14 *func_800D0180(Link800DBF14 *link);
extern void func_800DAA58(unsigned char id, void *link);
Obj800DBF14 *func_800DBF14(Obj800DBF14 *self, unsigned char *params) {
    Link800DBF14 *link;
    func_800DA904(self, 0x15, params++);
    link = &self->link_10;
    self->vtable_04 = D_80158568;
    func_800D0180(link);
    func_800DAA58(*params, link);
    return self;
}
