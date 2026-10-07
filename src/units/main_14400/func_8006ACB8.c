#include "common.h"

typedef unsigned char u8;
typedef struct Node {
    struct Node *next;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
    u8 field_9;
    u8 pad_A[2];
    u32 tag;
    u8 pad_10[4];
    u8 buffer[0x60];
} Node;
extern Node D_801D92BC;
extern Node *D_8003729C;
void func_800265E0(void *dst, s32 size);
u32 func_8002AF70(void);
void func_8002AFE0(u32 mask);

Node *func_8006ACB8(void) {
    Node *node;
    u32 mask;

    if (D_801D92BC.tag == 0xA8000000) {
        return &D_801D92BC;
    }
    D_801D92BC.field_4 = 3;
    D_801D92BC.field_5 = 5;
    D_801D92BC.field_8 = 12;
    D_801D92BC.field_6 = 13;
    D_801D92BC.field_7 = 2;
    D_801D92BC.tag = 0xA8000000;
    D_801D92BC.field_9 = 1;
    func_800265E0(D_801D92BC.buffer, 0x60);
    node = &D_801D92BC;
    mask = func_8002AF70();
    node->next = D_8003729C;
    D_8003729C = node;
    func_8002AFE0(mask);
    return &D_801D92BC;
}
