#include "common.h"

typedef struct OSPfs OSPfs;
typedef struct ControllerStorage { OSPfs *filesystem; } ControllerStorage;
typedef struct ControllerView {
    unsigned char pad_00[0xC];
    ControllerStorage *storage_0C;
} ControllerView;
extern s32 func_8002F420(OSPfs *pfs);

s32 func_80131544(ControllerView *controller)
{
    return func_8002F420(controller->storage_0C->filesystem);
}
