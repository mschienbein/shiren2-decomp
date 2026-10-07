#include "common.h"
#include "pi_handle_view.h"

extern void func_8002F704(void);
extern void func_8002F770(void);

/* osEPiWriteIo; uses the raw writer's PiHandleView/PiWord/PiResult contract. */
PiResult func_80029FB0(PiHandleView *handle, PiWord devAddr, PiWord data)
{
    PiResult ret;

    func_8002F704();
    ret = func_80029DE0(handle, devAddr, data);
    func_8002F770();
    return ret;
}
