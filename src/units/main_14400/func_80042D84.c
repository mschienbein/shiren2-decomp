#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos;

typedef struct {
    Pos a;
    Pos b;
} Rect;

/* Floor room record (0x20 bytes): two corners stored transposed, then four
 * words whose low byte is copied out. */
typedef struct {
    Pos a;
    Pos b;
    struct {
        u8 pad[3];
        u8 value;
    } items[4];
} Room;

typedef struct {
    u8 pad0[0xF];
    u8 room_count;
    Room *rooms;
} Floor;

typedef struct {
    Floor *floor;
} MenuState;

/* Runtime room table (0x14-byte entries). */
typedef struct {
    Rect rect;
    u8 values[4];
} RoomInfo;

typedef struct {
    u8 pad0[0xC];
    s32 floor_id;
} Arg;

extern u8 D_8014344C;
extern RoomInfo D_801431F0[];
extern unsigned short D_80143450[][0x4C]; /* floor cell map */

MenuState *func_80066B20(void);
void func_80066B2C(void *map, s32 floor_id);
void func_800B17A4(void);
s32 func_80049CB4(s32 id, ...);

void func_80042D84(Arg *arg) {
    MenuState *state = func_80066B20();
    s32 i;
    Rect rect;

    func_80066B2C(D_80143450, arg->floor_id);
    D_8014344C = state->floor->room_count;
    for (i = 0; ; i++) {
        Room *room;
        s32 k;

        if (i >= D_8014344C) {
            break;
        }
        room = &state->floor->rooms[i];
        /* Transpose both corners into (x, y) order. */
        {
            Pos *corner = &rect.a;
            s32 y = room->a.y;
            s32 x = room->a.x;
            corner->x = y;
            corner->y = x;
        }
        {
            s32 y = room->b.y;
            s32 x = room->b.x;
            rect.b.x = y;
            rect.b.y = x;
        }
        {
            RoomInfo *info = &D_801431F0[i];
            info->rect.a = rect.a;
            info->rect.b = rect.b;
        }
        for (k = 0; k < 4; k++) {
            D_801431F0[i].values[k] = room->items[k].value;
        }
    }
    func_800B17A4();
    func_80049CB4(5, 1);
}
