#include "common.h"
typedef short s16;
typedef struct Config801303A0 Config801303A0;
/* Allocated driver header followed by variable pointer slots (GNU trailing array). */
typedef struct { unsigned char header_00[0x24]; void *slots_24[0]; } Driver;
typedef struct { unsigned char pad_00[0x34]; Driver *driver_34; } Context;
extern Context *D_80148D84;
extern void func_8012DB00(void **slot, Config801303A0 *cfg, void *heap);
void *func_80130930(s16 index, Config801303A0 *cfg, void *heap) {
    func_8012DB00(&D_80148D84->driver_34->slots_24[index], cfg, heap);
    return D_80148D84->driver_34->slots_24[index];
}
