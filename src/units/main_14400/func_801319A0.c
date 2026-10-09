#include "common.h"
/* The four 10-byte channel slots at D_801D4CDC (same table as func_80131AFC);
 * D_801D4CE0/D_801D4CE2/D_801D4CE3 are splat labels of slot 0's +4/+6/+7
 * members, not objects: index the real table. */
typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short counter;
    unsigned char flag;
    unsigned char state_7;
    unsigned char pad8[2];
} Slot;
typedef struct Node Node;
extern Slot D_801D4CDC[4];
extern Node D_80148E60;
extern void func_80131BA0(Node *node);
void func_801319A0(void) {
    u32 i;
    for (i = 0; i < 4; i++) {
        D_801D4CDC[i].flag = 2;
        D_801D4CDC[i].state_7 = 0;
        D_801D4CDC[i].counter = i;
    }
    func_80131BA0(&D_80148E60);
}
