#include "common.h"

typedef struct OSPfs OSPfs;

/* Prefix view of the payload holder: +0 is the PFS handle. */
typedef struct {
    OSPfs *pfs;
} PfsHolder;

/* Prefix view of the free-blocks payload: holder at +0, output count at +4. */
typedef struct {
    PfsHolder *holder;
    s32 bytes_not_used;
} FreeBlocksPayload;

/* Dispatched request (func_80131CC0): command, reply queue, result, payload. */
typedef struct {
    s32 command;
    void *reply_queue;
    s32 result;
    FreeBlocksPayload *payload;
} Request;

extern s32 func_8002E610(OSPfs *pfs, s32 *bytes_not_used);

/* Handler table entry at 0x80148E10; the dispatcher stores the result in
 * request->result (0x80131E18-0x80131E1C), so the PFS status is returned. */
s32 func_80131378(Request *request) {
    FreeBlocksPayload *payload = request->payload;
    return func_8002E610(payload->holder->pfs, &payload->bytes_not_used);
}
