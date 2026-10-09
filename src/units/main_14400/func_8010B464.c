#include "common.h"
/* Stream reads at 0x800CE9F4..0x800CEA48 address this complete list header. */
typedef struct {
    void *table_00;
    void *vtable_04;
    unsigned char *items_08;
    unsigned char capacity_0C, start_0D, count_0E;
} List;
typedef struct { unsigned char pad_00[0xCC]; List container_CC; } Obj8010B464;
extern void func_800EE9C0(void *self, void *stream);
extern void func_800CE9C4(void *self, void *stream);
void func_8010B464(Obj8010B464 *self, void *stream) {
    func_800EE9C0(self, stream);
    func_800CE9C4(&self->container_CC, stream);
}
