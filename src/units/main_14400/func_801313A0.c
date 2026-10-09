#include "common.h"

typedef unsigned char u8;
typedef struct OSPfs OSPfs;
typedef struct { OSPfs *pfs; s32 file_no; } PfsFile;
typedef struct {
    PfsFile *file;
    s32 offset;
    s32 size;
    u8 flag;
    u8 pad0D[3];
    u8 *buffer;
} Request;
typedef struct { u8 pad00[0xC]; Request *request; } Message;
extern s32 func_8002EFE0(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, u8 *buffer);

s32 func_801313A0(Message *message)
{
    Request *request = message->request;
    return func_8002EFE0(request->file->pfs, request->file->file_no,
                        request->flag, request->offset, request->size, request->buffer);
}
