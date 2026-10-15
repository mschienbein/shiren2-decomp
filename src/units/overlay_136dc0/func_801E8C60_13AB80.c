#include "func_801E8C60_13AB80/types.h"
extern char *func_80083C90(char *dst, char *src);
void func_801E8C60_13AB80(void *self, s32 index, char *text) {
    S06Widget *widget = self;
    s32 value = widget->table4C->call6C((u8 *)widget + widget->table4C->delta68, index);
    S06Event *event = D_801F51E8_147108;
    func_80083C90(text, (char *)event + (event->offsets9C8[value] + 0x126));
}
