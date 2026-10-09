#include "common.h"

typedef unsigned char u8;

typedef struct Stream Stream;

typedef struct { s32 x0; s32 x4; } Ent;
typedef struct { u8 pad[0x18]; u32 x18; Ent *x1C; } Tbl;

extern void *func_80091450(u32 size);
extern s32 func_8008DF04(Stream *stream);

/* Allocates the table's x18 entries and reads each entry's first word from the stream;
 * returns the bytes read or -1 when the allocation fails. */
s32 func_80090A04(Stream *stream, Tbl *t)
{
    s32 err = 0;
    s32 size = 0;
    u32 i;

    /* ODD_C: groups allocation and read; a failed allocation breaks to the shared size = -1 exit.
     * Also shapes scheduling: the if/else and goto-done forms each differ in 5 words. */
    do {
        t->x1C = func_80091450(t->x18 * 8);
        if (t->x1C == 0) {
            err = -1;
            break;
        }
        for (i = 0; i < t->x18; i++) {
            t->x1C[i].x0 = func_8008DF04(stream);
            size += 4;
        }
    } while (0);
    if (err) size = -1;
    return size;
}
