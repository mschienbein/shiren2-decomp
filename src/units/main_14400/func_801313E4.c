#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct OSPfs OSPfs;
typedef struct { OSPfs *pfs; s32 file_no; } PfsFile;
typedef struct {
    PfsFile *file;
    u8 *game_name;
    u8 *ext_name;
    s32 size;
    s32 mode;
} Request;
typedef struct { u8 pad00[0xC]; Request *request; } Message;
extern u16 D_80148E00[2];
extern s32 D_80148E04;
extern s32 func_8002F480(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 func_8002D700(OSPfs *, u16, u32, u8 *, u8 *, s32, s32 *);

static inline u16 company_code(const u16 *value) { return value[0]; }

s32 func_801313E4(Message *message)
{
    Request *request = message->request;
    PfsFile *file = request->file;
    s32 result;
    result = func_8002F480(file->pfs, company_code(D_80148E00), D_80148E04,
                          request->game_name, request->ext_name, &file->file_no);
    if (result == 5 && request->mode == 1) {
        result = func_8002D700(file->pfs, company_code(D_80148E00), D_80148E04,
                              request->game_name, request->ext_name, request->size,
                              &file->file_no);
    }
    return result;
}
