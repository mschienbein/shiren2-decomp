#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct Node8012FDE0 { struct Node8012FDE0 *next; u8 pad4[0xC]; s32 field_10; } Node8012FDE0;
typedef struct { Node8012FDE0 *head; u8 pad4[0x1C]; s32 field_20; } List8012FDE0;
extern List8012FDE0 *D_80148D84;
u32 func_80031F90(u32 arg0);
void func_8012FDE0(Node8012FDE0 *node) {
    u32 saved = func_80031F90(1);
    List8012FDE0 *list = D_80148D84;
    node->field_10 = list->field_20;
    node->next = list->head;
    list->head = node;
    func_80031F90(saved);
}
