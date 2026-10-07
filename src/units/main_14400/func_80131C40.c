#include "common.h"
typedef struct Node { struct Node *next; } Node;
extern Node *D_80148E70;
u32 func_80031F90(u32);
void func_80131C40(Node *target){
    Node **link = &D_80148E70;
    Node *cur;
    if (*link != 0) {
        do {
            cur = (*link)->next;
            if (cur == target) {
                u32 mask = func_80031F90(1);
                (*link)->next = cur->next;
                cur->next = 0;
                func_80031F90(mask);
                return;
            }
            link = &(*link)->next;
        } while (cur != 0);
    }
}
