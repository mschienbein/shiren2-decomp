#include "common.h"

typedef struct Task Task;
struct Task { void (*callback)(Task *); unsigned short state; char pad6[8]; unsigned short done; unsigned short parent; unsigned short flags; char pad14[0x60]; };
extern Task D_801BA380[];
extern s32 D_8013E910;
s32 func_80084E20(void) {
    s32 active,i;
    D_8013E910=1;
    active=0;
    i=0;
    do {
        Task *task=&D_801BA380[i];
        void (*callback)(Task *)=task->callback;
        if(callback) {
            unsigned short parentIndex=task->parent;
            unsigned short state=task->state;
            Task *parent=&D_801BA380[parentIndex];
            switch(state) {
            case 1:
                callback(task);
                if(task->state==4) { task->callback=0; task->done=1; }
                else if(!(task->flags&0x8000)) active++;
                break;
            case 2:
                if(!(task->flags&0x8000)) active++;
                if(parentIndex==i || (parent->done&1)) task->state=1;
                if(task->state==1) {
                    task->callback(task);
                    if(task->state==4) { task->callback=0; task->done=1; }
                    else if(!(task->flags&0x8000)) active++;
                }
                break;
            case 3:
                task->callback=0;
                break;
            }
        }
        i++;
    } while(i<0xAE);
    D_8013E910=0;
    return active;
}
