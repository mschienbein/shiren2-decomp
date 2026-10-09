#include "common.h"
/* 0x30-byte item record (func_800AFD78 stride), copied whole here: kind byte at
 * +0 (func_800AE648), content-list owner pointer at +0x24 (func_8011422C).
 * Other bytes are not interpreted by this function. */
typedef struct { unsigned char kind00; unsigned char pad01[0x23]; void *owner24; unsigned char pad28[8]; } Entry;
/* Item table: record array +0, occupancy bitset +4 (func_800AF920), count +8. */
typedef struct { Entry *items; unsigned char *bitset; s32 field08; } Container;
/* +4 is not accessed here. */
typedef struct { Container *field00; unsigned char pad04[4]; s32 field08; } Owner;
extern s32 func_800AF920(Container *container, unsigned char index);
extern Entry *func_800AFD78(Container *container, unsigned char index);
extern void func_800AFCD8(Container *container, unsigned char index);
extern void func_800AFCA8(Container *container, unsigned char index);
extern void func_800AE648(Entry *entry);
static inline s32 needs_move(Container *container, unsigned char index) {
    return func_800AF920(container, index) ^ 1;
}
void func_800D0E38(Owner *owner) {
    s32 source = 0;
    s32 destination;
    s32 count;
    Entry *entry;
    if (owner->field08 != 0) {
        count = owner->field00->field08;
        destination = 0;
        while (1) {
            if (destination < count - 1) {
                if (needs_move(owner->field00, destination)) {
                    if (destination >= source) {
                        source = destination + 1;
                    }
                    if (source >= count) {
                        return;
                    }
                    while (func_800AF920(owner->field00, source) == 0) {
                        source++;
                        if (source >= count) {
                            break;
                        }
                    }
                    if (source >= count) {
                        return;
                    }
                    entry = func_800AFD78(owner->field00, destination);
                    *entry = *func_800AFD78(owner->field00, source);
                    func_800AFCD8(owner->field00, destination);
                    func_800AFCA8(owner->field00, source++);
                    func_800AE648(entry);
                }
                destination++;
            } else {
                owner->field08 = 0;
                break;
            }
        }
    }
}
