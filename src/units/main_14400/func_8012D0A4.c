#include "common.h"
#include "controller_queue_view.h"

typedef struct Node Node;
struct Node { Node *prev; Node *next; s32 timer; };
extern s32 D_801CA748;
extern ControllerQueueView D_801CA74C;
extern Node *D_801CA730;
extern Node *D_801CA734;
ControllerQueueS32 func_8002FEA0(ControllerQueueView *, ControllerQueueMessage *, ControllerQueueS32);
void func_8012D0A4(void) {
    ControllerQueueMessage msg;
    Node *node;
    Node *next;
    while (D_801CA748 != 0) {
        func_8002FEA0(&D_801CA74C, &msg, 0);
        D_801CA748--;
    }
    node = D_801CA730;
    while (node != 0) {
        if (--node->timer == 0) {
            next = node->next;
            if (next != 0) {
                next->prev = node->prev;
            }
            if (node->prev != 0) {
                node->prev->next = node->next;
            } else {
                D_801CA730 = node->next;
            }
            node->prev = 0;
            node->next = D_801CA734;
            D_801CA734 = node;
            node = next;
        } else {
            node = node->next;
        }
    }
}
