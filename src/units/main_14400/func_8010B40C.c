#include "common.h"

typedef unsigned char u8;

/* func_8010B34C allocates 0xE4 bytes; func_8010B220 constructs the list at 0xCC. */
typedef struct { u8 pad0[0xCC]; u8 sub_CC[0x18]; } Obj8010B40C;
void func_800CE3E8(void *sub, void *list);

void func_8010B40C(Obj8010B40C *obj, void *list) {
    func_800CE3E8(obj->sub_CC, list);
}
