#include "common.h"

/* Views of the resident message-queue interface (func_80027EA0 = create queue). */
typedef void *ProbeMessage;
typedef struct ProbeMessageQueue ProbeMessageQueue;

extern void func_80027EA0(ProbeMessageQueue *queue, ProbeMessage *messages, long capacity);

extern ProbeMessageQueue D_80161684;
extern ProbeMessage D_8016169C[4];

void func_80052D5C(void) {
    func_80027EA0(&D_80161684, D_8016169C, 4);
}
