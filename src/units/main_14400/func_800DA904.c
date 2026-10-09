#include "common.h"

/* Eight-byte collection/item link at +8: zeroed by func_800D0180, then filled by
 * func_800DAA58 through func_800D01B8. */
typedef struct {
    void *collection;
    void *item;
} Link;

typedef struct {
    short unk00;
    const void *unk04;
    Link link;
} Object;
extern const unsigned char D_80157FA8[]; /* 0x30-byte command vtable. */
extern const unsigned char D_80158388[]; /* 0x30-byte command vtable. */
extern void *func_800D0180(void *link);
extern void func_800DAA58(unsigned char id, void *link_out);

Object *func_800DA904(Object *object, s32 kind, unsigned char *params) {
    Link *link = &object->link;
    object->unk04 = &D_80157FA8;
    object->unk00 = kind;
    object->unk04 = &D_80158388;
    func_800D0180(link);
    func_800DAA58(*params, link);
    return object;
}
