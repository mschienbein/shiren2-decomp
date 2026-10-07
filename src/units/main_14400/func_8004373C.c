#include "common.h"

/* Opaque view of the PI handle node returned by func_8006ACB8. */
typedef struct Node Node;

extern Node *D_80138B08;
Node *func_8006ACB8(void);
void func_80043768(void);

void func_8004373C(void) {
    D_80138B08 = func_8006ACB8();
    func_80043768();
}
