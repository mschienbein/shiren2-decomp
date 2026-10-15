#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
struct Pos_8010B5D0 { s32 x, y; };
struct Methods_8010B5D0 { u8 pad_00[0x98]; short delta_98, index_9A; void *(*inventory_9C)(void *); };
struct Actor_8010B5D0 { u8 pad_00[0x24]; Methods_8010B5D0 *table_24; };
struct Collection_8010B5D0 { void *owner, *table; u8 pad_08[0x14]; };
struct Object_8010B5D0 { u8 pad_00[0xCC]; Collection_8010B5D0 collection_CC; };
/* Item transfer request filled by func_800A1270 and run by func_800A1308;
   +0x10 receives the selected item. */
struct Request_8010B5D0 { void *from, *to; u16 message; s32 (*filter)(void *); void *selected; };
/* The caller's frame reserves eight bytes for the selection result at sp+0x18,
   before Menu at sp+0x20. Only index is written/read on this menu path; nested
   menus use later words for selection-path entries, not an observed detail. */
struct Selection_8010B5D0 { s32 index; s32 value; };
struct Menu_8010B5D0;
extern "C" {
extern Actor_8010B5D0 *D_801476B8;
extern Pos_8010B5D0 D_80138FD0;
extern const u8 D_80149D18[];
extern const u8 D_80151E38[144];
Menu_8010B5D0 *func_800953C0(Menu_8010B5D0 *);
void func_8009A930(Menu_8010B5D0 *, Pos_8010B5D0 *);
s32 func_800957C0(void *, void *, s32, void *, s32);
void *func_800A1270(void *, void *, void *, u16, s32 (*)(void *));
s32 func_800A1308(void *, u16);
s32 func_8010B840(void *);
void func_800498E4(s32, ...);
void func_80049A04(u16, ...);
void func_80049BF0(s32);
s32 func_80049CB4(s32, ...);
}
/* Two-entry choice menu: base constructor func_800953C0, then its own table;
   the destructor restores the base table on every exit path. */
struct Menu_8010B5D0 {
    u8 pad_00[0x4C];
    const void *table_4C;
    u8 pad_50[0xC];
    Menu_8010B5D0() { func_800953C0(this); table_4C = D_80149D18; }
    ~Menu_8010B5D0() { table_4C = D_80151E38; }
    s32 choose(Selection_8010B5D0 *selection) { return func_800957C0(this, selection, 1, 0, 0) == 1; }
};

/* Moves one item between the player's inventory and the object's collection
   (direction chosen from the menu); returns the moved item or 0. Each
   direction retries its request after a recoverable refusal (status 3/5). */
extern "C" void *func_8010B5D0(Object_8010B5D0 *object) {
    void *inventory = D_801476B8->table_24->inventory_9C((u8 *)D_801476B8 + D_801476B8->table_24->delta_98);
    Selection_8010B5D0 selection;
    Menu_8010B5D0 menu;
    func_8009A930(&menu, &D_80138FD0);
    if (!menu.choose(&selection)) return 0;
    switch (selection.index) {
    case 1: {
        s32 repeat = 0;
    retry_store:
        {
            Request_8010B5D0 request;
            func_800A1270(&request, inventory, &object->collection_CC, 0x1F5, func_8010B840);
            switch (func_800A1308(&request, 0x1FE)) {
            case 0: func_80049A04(0x1F6); break;
            case 1: func_80049A04(0x1F7); break;
            case 3: func_800498E4(0x1F8); repeat = 1; func_80049BF0(0); break;
            case 5: func_800498E4(0x1C8); repeat = 1; func_80049BF0(0); break;
            case 7:
                func_80049A04(0x1F9);
                func_80049CB4(2);
                return request.selected;
            }
            func_80049CB4(2);
            if (repeat) {
                func_80049CB4(0x129, 10);
                repeat = 0;
                goto retry_store;
            }
        }
        break;
    }
    case 2: {
        s32 repeat = 0;
    retry_take:
        {
            Request_8010B5D0 request;
            func_800A1270(&request, &object->collection_CC, inventory, 0x1F5, 0);
            /* FAKEMATCH: repeat is zero on every entry here; reusing it as the
               disabled prompt id contributes no prompt semantics, but keeps
               the original move a1,s1 instead of move a1,zero. Literal 0
               misses one word at +0x19C: scratch/omp/wave10/g02/try/b5d0_p5.cpp,
               receipt scratch/omp/wave10/g02/tryres/b5d0_p5/result.json. */
            switch (func_800A1308(&request, repeat)) {
            case 0: func_80049A04(0x1FA); break;
            case 1: func_80049A04(0x1FB); break;
            case 3: func_800498E4(0x1FC); func_80049BF0(0); repeat = 1; break;
            /* ODD_C: status 5 (seen by the store direction above) is a real
               no-op outcome here; listing it keeps the original dense
               dispatch table. */
            case 5: break;
            case 7:
                func_80049A04(0x1FD);
                func_80049CB4(2);
                return request.selected;
            }
            func_80049CB4(2);
            if (repeat) {
                func_80049CB4(0x129, 10);
                repeat = 0;
                goto retry_take;
            }
        }
        break;
    }
    }
    return 0;
}
