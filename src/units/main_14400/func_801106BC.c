#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Partial view of the externally owned unit. */
typedef struct { u8 pad0[0x1E]; u8 x1E; } Unit;
typedef struct { Unit *unit; signed char x4; } InfoEntry;
/* One 0xA8-byte record: state at 0x18, flags through 0x26, entries at 0x28. */
typedef struct {
    Unit *unit;
    s32 position[2];
    u8 direction;
    s32 nextPosition[2];
    u32 state;
    u8 flags1C[11];
    InfoEntry entries[16];
} Info;
void func_80136908(u32 *state);
void func_8010F020(void *obj, Info *info, Unit *unit);
void func_80110748(void *obj, Unit *unit, Info *info);
u16 func_800E08B0(Unit *unit);
void func_801110A8(void *obj, Unit *unit, Info *info);
void func_801106BC(void *obj, Unit *unit) {
    Info info;
    Info *infoPtr;
    if (unit->x1E & 0xC) {
        func_80136908(&info.state);
        infoPtr = &info;
        func_8010F020(obj, infoPtr, unit);
        func_80110748(obj, unit, infoPtr);
        if (func_800E08B0(unit)) {
            func_801110A8(obj, unit, infoPtr);
        }
    }
}
