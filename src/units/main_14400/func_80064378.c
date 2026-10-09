#include "common.h"
typedef struct Node { struct Node *field_00; void *field_04; } Node;
extern Node *D_8016DB50;
extern void *func_80062CCC(void *value, void *base, u32 tag);
void func_80064378(Node *base, u32 tag) {
    Node *node;
    if (base == 0) { D_8016DB50 = 0; return; }
    node = func_80062CCC(base->field_00, base, tag);
    D_8016DB50 = node;
    if (node->field_00 != 0) {
        node = func_80062CCC(node->field_00, base, tag);
        D_8016DB50->field_00 = node;
        if (node->field_00 != 0) D_8016DB50->field_00->field_00 = func_80062CCC(node->field_00, base, tag);
        if (D_8016DB50->field_00->field_04 != 0)
            D_8016DB50->field_00->field_04 = func_80062CCC(D_8016DB50->field_00->field_04, base, tag);
    }
}
