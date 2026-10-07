#include "common.h"

typedef struct { char data[0x18]; } MesgQueue;
typedef struct { char data[0x10]; } Node;
void func_80027EA0(MesgQueue *queue, void *messages, long capacity);
void func_8006C350(Node *node, MesgQueue *mq, unsigned char flags);
long func_8002FEA0(MesgQueue *queue, void *message, long flags);
void func_8006C418(Node*);
void func_8006C494(s32 count){
    Node node;
    MesgQueue queue;
    void *msg[2];
    func_80027EA0(&queue, msg, 1);
    func_8006C350(&node, &queue, 1);
    for (; count != 0; count--) func_8002FEA0(&queue, 0, 1);
    func_8006C418(&node);
}
