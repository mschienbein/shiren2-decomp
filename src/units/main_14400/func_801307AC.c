#include "common.h"
typedef struct Node { struct Node *next; } Node;
typedef struct { unsigned char field_00[0x2C]; Node *field_2C; } Owner;
extern Owner *D_80148D84;
void func_801307AC(Node *node) { node->next = D_80148D84->field_2C; D_80148D84->field_2C = node; }
