#include "common.h"
typedef struct { unsigned char field_0[0x16]; short field_16; } Priority;
typedef struct Node { struct Node *field_0; s32 field_4; Priority *field_8; unsigned char field_C[0x7C]; s32 field_88; } Node;
typedef struct { s32 field_0; Node *field_4; s32 field_8; Node *field_C; s32 field_10; Node *field_14; } Context;
extern Context *D_80148D84;
extern void func_800326CC(Node *);
extern void func_800326AC(Node *,Node **);
s32 func_8012FF48(Node **out,short priority) {
    Node *node=D_80148D84->field_14;
    s32 found=0;
    if(!node) node=D_80148D84->field_4;
    if(node) {
        *out=node;
        func_800326CC(node);
        func_800326AC(node,&D_80148D84->field_C);
    } else {
        for(node=D_80148D84->field_C;node;node=node->field_0) {
            if(priority >= node->field_8->field_16 && !node->field_88) { *out=node; found=1; priority=node->field_8->field_16; }
        }
    }
    return found;
}
