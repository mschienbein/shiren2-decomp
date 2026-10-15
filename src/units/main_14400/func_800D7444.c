#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
extern u8 D_801480B0[];
/* Same unsigned-halfword view as func_800D73D0/D74AC/D76E0 (read back with lhu). */
extern unsigned short D_801480CC;
extern u8 D_801480CE;
extern u8 D_801480CF;
void func_800D7444(unsigned short arg0) {
    u8 *entry = D_801480B0;
    D_801480CF = 1;
    D_801480CE = 0;
    for (;;) {
        u8 end;
        if (entry[0] == 0) {
            break;
        }
        end = entry[0] + entry[1];
        if (D_801480CE < end) {
            D_801480CE = end;
        }
        entry += 3;
    }
    D_801480CC = arg0;
}
