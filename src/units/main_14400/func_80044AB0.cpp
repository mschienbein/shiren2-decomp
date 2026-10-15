#include "common.h"
inline void *operator new(unsigned int, void *p) { return p; }

typedef unsigned char u8;

extern "C" u8 *func_8006A810(u8 *dst, s32 value, s32 count);

/* 12-byte record {id, x, y} (layout of func_80044768.c End).  A POD view:
   the null record D_80138B40 is C-owned storage, so it is used through this
   constructor-free type rather than as a constructed End. */
struct EndStorage {
    s32 id;
    s32 x;
    s32 y;
    void clear() { func_8006A810((u8 *)this, 0, sizeof(EndStorage)); }
};

/* Constructed record, used for the pair members. */
struct End : EndStorage {
    End() {}
    End(s32 i, s32 px, s32 py)
    {
        clear();
        id = i;
        x = px;
        y = py;
    }
};

extern "C" EndStorage D_80138B40;

/* 24-byte record pair; the second record is filled member by member. */
struct EndPair {
    End first;
    End second;
    EndPair(s32 i, s32 x, s32 y) : first(i, x, y)
    {
        second.id = D_80138B40.id;
        second.x = D_80138B40.x;
        second.y = D_80138B40.y;
    }
    EndPair(s32 i0, s32 x0, s32 y0, s32 i1, s32 x1, s32 y1) : first(i0, x0, y0)
    {
        second.clear();
        second.id = i1;
        second.x = x1;
        second.y = y1;
    }
};

extern "C" {
extern EndPair D_80138B4C;
extern EndPair D_80138B64;
extern EndPair D_80138B7C;
extern EndPair D_80138B94;
}

/* Static initialiser: the null record {-1, 0, 0}, then the four pairs. */
extern "C" void func_80044AB0(void)
{
    D_80138B40.clear();
    D_80138B40.id = -1;
    D_80138B40.x = 0;
    D_80138B40.y = 0;
    new (&D_80138B4C) EndPair(0x55, 0xF, 0x33);
    new (&D_80138B64) EndPair(0x56, 0x19, 0x2F, 0x57, 0xF, 0xF);
    new (&D_80138B7C) EndPair(0x58, 0x13, 0x32, 0x59, 8, 0x16);
    new (&D_80138B94) EndPair(0x59, 0x24, 0x31);
}
