#include "common.h"

typedef struct OSPfs OSPfs;
typedef struct OSPfsState OSPfsState;

typedef struct {
    OSPfs *pfs;
    s32 file_no;
} PfsFile;

typedef struct {
    PfsFile *file;
    OSPfsState *state;
} PfsArgs;

typedef struct {
    char pad00[0xC];
    PfsArgs *args;
} PfsRequest;

extern s32 func_8002E470(OSPfs *pfs, s32 file_no, OSPfsState *state);

s32 func_801314E4(PfsRequest *req)
{
    PfsArgs *args = req->args;

    return func_8002E470(args->file->pfs, args->file->file_no, args->state);
}
