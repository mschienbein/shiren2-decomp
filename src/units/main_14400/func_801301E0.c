#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef short s16;

typedef struct {
    u8 pad0[0x88];
    s32 field_88;
} Owner_801301E0;

typedef struct {
    Owner_801301E0 *field_0;
    Owner_801301E0 *field_4;
    Owner_801301E0 *field_8;
} Obj_801301E0;

typedef struct Node_801301E0 {
    struct Node_801301E0 *next;
    s32 field_4;
    s16 field_8;
    s32 field_C;
    s32 field_10;
} Node_801301E0;

typedef struct {
    u8 pad0[0x1C];
    s32 field_1C;
} Global_801301E0;

extern Global_801301E0 *D_80148D84;
extern Node_801301E0 *func_80130780(void);
extern s32 func_801308B4(s32 arg, s32 offset);
extern s32 func_8012E5F8(Owner_801301E0 *owner, s32 kind, Node_801301E0 *node);

void func_801301E0(Obj_801301E0 *obj, s16 value, s32 arg) {
    Node_801301E0 *node;
    s32 offset;

    if (obj->field_8 != 0) {
        node = func_80130780();
        if (node != 0) {
            offset = D_80148D84->field_1C + obj->field_8->field_88;
            node->field_8 = 0xB;
            node->field_C = value;
            node->field_4 = offset;
            node->field_10 = func_801308B4(arg, offset);
            node->next = 0;
            func_8012E5F8(obj->field_8, 3, node);
        }
    }
}
