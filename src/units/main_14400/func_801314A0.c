#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct OSPfs OSPfs;
extern u16 D_80148E00[2];
extern s32 D_80148E04;
typedef struct { OSPfs **pfs; u8 *game_name; u8 *ext_name; } Request;
typedef struct { u8 pad00[0xC]; Request *request; } Message;
extern s32 func_8002E250(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);

s32 func_801314A0(Message *message)
{
    Request *request = message->request;
    return func_8002E250(*request->pfs, D_80148E00[0], D_80148E04,
                        request->game_name, request->ext_name);
}
