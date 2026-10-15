#include "common.h"
#include "map_rect.h"

typedef struct {
    MapRect geometry;
    unsigned char side_counts[4];
} Entry;


typedef struct {
    unsigned char pad0[0x3DC];
    s32 unk3DC;
    unsigned char pad3E0[0x578];
    unsigned short unk958;
} Obj;

extern Entry D_801431F0[];
extern s32 func_800B68B0(Entry *entry);
extern s32 func_800BBD08(Obj *obj, Entry *entry, s32 index);
extern MapPos *func_800B6A98(MapPos *out, Entry *room, s32 index);
extern s32 func_800B69F4(MapRect *entry, MapPos *cell);
extern void func_800BC2AC(Obj *obj, MapPos *cell, unsigned char value);

s32 func_800BC49C(Obj *obj) {
    s32 i;
    s32 j;
    s32 count;
    Entry *entry;
    MapPos cell;

    if (!(obj->unk958 & 0x400)) {
        return 0;
    }
    i = 0;
    entry = D_801431F0;
    while (1) {
        if (i >= obj->unk3DC) {
            return 1;
        }
        count = func_800B68B0(entry);
        if (count != 0) {
            j = 0;
            while (1) {
                if (j >= count) {
                    break;
                }
                if (func_800BBD08(obj, entry, j)) {
                    unsigned char value;

                    func_800B6A98(&cell, entry, j);
                    value = func_800B69F4(&entry->geometry, &cell) * 2;
                    func_800BC2AC(obj, &cell, value);
                }
                j++;
            }
        }
        entry++;
        i++;
    }
}
