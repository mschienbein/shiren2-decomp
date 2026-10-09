#include "common.h"
/* RecordObject prefix includes the genuine child pointer cleared at +0x1C. */
typedef struct RecordChild RecordChild;
typedef struct { unsigned char pad0[9], flags9; signed char valueA; unsigned char padB[0x11]; RecordChild *child1C; unsigned char value20; } RecordObject;
void *func_800CA760(RecordObject *p) { p->child1C = 0; return p; }
