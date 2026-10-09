#include "common.h"

typedef struct OSPfs OSPfs;
typedef struct { OSPfs *pfs; } FileSystem;
typedef struct { FileSystem *fs; s32 *maxFiles; s32 *filesUsed; } Request;
typedef struct { unsigned char pad00[0xC]; Request *request0C; } Obj;
extern s32 func_8002EEE0(OSPfs *pfs, s32 *max_files, s32 *files_used);

s32 func_80131514(Obj *obj)
{
    Request *request = obj->request0C;
    return func_8002EEE0(request->fs->pfs, request->maxFiles, request->filesUsed);
}
