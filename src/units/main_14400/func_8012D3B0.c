#include "common.h"
#include "controller_queue_view.h"

extern char *D_801CA774;
extern void *D_801CA770;
extern char *func_8012D84C(s32);
extern void func_80027EA0(void *,void *,ControllerQueueS32),func_80031488(void *,void *,void *);
void func_8012D3B0(void) { char *obj=func_8012D84C(0x58); D_801CA774=obj; func_80027EA0(obj+8,obj+0x20,4); func_80027EA0(D_801CA774+0x30,D_801CA774+0x48,4); func_80031488(D_801CA770,D_801CA774,D_801CA774+8); }
