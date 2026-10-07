#include "common.h"
#include "controller_queue_view.h"
extern char *D_801CA774;
extern ControllerQueueS32 func_8002FEA0(void*,void*,ControllerQueueS32);
void func_8012D414(void) { short *message; do { func_8002FEA0(D_801CA774+8,&message,1); func_8002FEA0(D_801CA774+8,0,0); } while(*message!=1); }
