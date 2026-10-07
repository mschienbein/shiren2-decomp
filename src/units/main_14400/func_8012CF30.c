#include "common.h"
#include "controller_queue_view.h"

typedef u32 (*Func8012DmaProc)(u32 address, s32 length, void *state);
typedef Func8012DmaProc (*Func8012DmaFactory)(void **state);

typedef struct Node {
    struct Node *prev0;
    struct Node *next4;
    s32 x8;
    s32 idC;
    void *buf10;
} Node;
extern void *D_801CA764;
extern void *D_801CA73C;
extern void *D_801CA740;
extern Node *D_801CA738;
extern Node *D_801CA734;
extern s32 D_801CA744;
extern s32 D_801CA748;
extern Node *D_801CA730;
extern ControllerQueueView D_801CA74C;
void *func_80026680(void);
void *func_8012D84C(s32 size);
void func_8012D8A4(void *destination, s32 value, u32 count);
void func_80027EA0(ControllerQueueView *queue, ControllerQueueMessage *messages, ControllerQueueS32 capacity);
Func8012DmaProc func_8012D178(void **state);
static inline void linkNext(Node *n)
{
    n->next4 = n + 1;
    n[1].prev0 = n;
}

Func8012DmaFactory func_8012CF30(s32 count, s32 bufSize) {
    s32 i;
    D_801CA764 = func_80026680();
    D_801CA73C = func_8012D84C(count * 0x30);
    D_801CA740 = func_8012D84C(count * 8);
    D_801CA738 = func_8012D84C(count * sizeof(Node));
    func_8012D8A4(D_801CA738, 0, count * sizeof(Node));
    for (i = 0; i < count - 1; i++) {
        linkNext(&D_801CA738[i]);
        D_801CA738[i].buf10 = func_8012D84C(bufSize);
        D_801CA738[i].idC = -1;
    }
    D_801CA738[i].buf10 = func_8012D84C(bufSize);
    D_801CA738[i].idC = -1;
    D_801CA744 = bufSize;
    D_801CA748 = 0;
    D_801CA730 = 0;
    D_801CA734 = D_801CA738;
    func_80027EA0(&D_801CA74C, D_801CA740, count * 2);
    return func_8012D178;
}
