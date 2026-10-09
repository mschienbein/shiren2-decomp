#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 id; u16 first, second; } PairEntry;
typedef struct { s32 id; u16 first, second, alternate; u16 padA; } TripleEntry;
extern PairEntry D_80153344[48];
extern TripleEntry D_801534C4[23];

s32 func_800A2910(u8 kind, u8 id, u16 *out) {
    if ((u8)(kind - 3) >= 2) {
        return 0;
    }
    if ((kind == 3 && (u8)(id - 0x32) < 0x27) ||
        (kind == 4 && (u8)(id - 0x59) < 0x1D)) {
        PairEntry *entry = D_80153344;
        s32 remaining;
        for (remaining = 47; remaining != -1; remaining--, entry++) {
            if (entry->id == id) {
                out[0] = entry->first;
                out[1] = entry->second;
                return 1;
            }
        }
    } else {
        TripleEntry *entry = D_801534C4;
        s32 remaining;
        for (remaining = 22; remaining != -1; entry++, remaining--) {
            if (entry->id == id) {
                out[0] = entry->first;
                if (kind == 3) {
                    out[1] = entry->second;
                } else {
                    out[1] = entry->alternate;
                }
                return out[1] != 0;
            }
        }
    }
    return 0;
}
