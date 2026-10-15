#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Same views as func_80067860: model node groups and the active-node table. */
typedef struct { s32 field_00; void *field_04, *field_08, *field_0C; } Tracks;
typedef struct { u8 pad_00[0x30]; Tracks *tracks_30; } Node;
typedef struct { Node ***groups; u32 count; } Model;
typedef struct { u8 bytes[0x4C]; } Animation;
typedef struct { Node *node; Animation animation; } ActiveNode;
/* 0x408-byte map-object group records. */
typedef struct { u8 field_00; u8 field_01; u8 field_02; u8 field_03[0x405]; } Group;
/* 0x14-byte slot handed out by func_800682BC (it sets used and clears +8/+C/+10). */
typedef struct {
    u8 used;
    u8 group;
    u8 active;
    u8 pad_03;
    u16 tile;
    u16 pad_06;
    Node *node;
    s32 field_0C;
    s32 field_10;
} Slot;

extern Group *D_801D40D4;
extern Model *D_801D2558;
extern ActiveNode *D_801D2C00;
extern u8 D_8016DC10;
void func_8006830C(s32 *tile, s32 *overlay, u8 x, u8 y);
Slot *func_800682BC(void);

void func_80068168(u8 group, u8 x, u8 y)
{
    /* ORIGINAL BUG: ids really has two elements (the tile and overlay IDs from
     * func_8006830C). The ROM gives it 8 bytes at sp+0x10..0x17 (0x8006817C/0x80068180),
     * saves s0 at sp+0x18 (0x800681A0), and the frame is 0x38. The loop condition below
     * reads ids[i] before testing i < 2, so after two full iterations it reads ids[2]. */
    s32 ids[2];
    u32 i;
    s32 id;
    Group *entry;

    func_8006830C(&ids[0], &ids[1], x, y);
    entry = &D_801D40D4[group];
    entry->field_02 = 0;
    entry->field_01 = 0;
    /* ORIGINAL BUG: the && tests the value first, so the bound does not guard the read.
     * When both IDs are real and both groups are non-null, the third test loads
     * sp+0x10+4*2 = sp+0x18 (0x80068288), which is the caller's s0 saved by this
     * function's prologue. That value does not matter: whether or not it equals 0xFFFF,
     * the loop exits (directly, or through the i < 2 test at 0x800681E0), and id is not
     * used after the loop. The read is harmless and stores nothing. */
    for (i = 0; (id = ids[i]) != 0xFFFF && i < 2; i++) {
        Node **list = D_801D2558->groups[id];
        Node *node;
        if (list == 0) {
            break;
        }
        while ((node = *list++) != 0) {
            Slot *slot = func_800682BC();
            u32 j;
            slot->group = group;
            slot->active = 0xFF;
            slot->tile = id;
            slot->node = node;
            for (j = 0; j < D_8016DC10; j++) {
                if (D_801D2C00[j].node == node) {
                    slot->active = j;
                }
            }
        }
    }
}
