#include "common.h"

/*
 * Party menu dispatcher (C++ TU, g++ 2.8.1).  Evidence for C++: every menu,
 * dialog and panel is a stack object whose method table sits at +0x4C after
 * its data (g++ 2.x vptr placement); the out-of-line base constructor is
 * func_800953C0 and the panel constructor func_80097AB0; destructors reset
 * the table to the root D_80151E38 (own-table resets that flow deletes as
 * dead stores are still counted by loop.c/global.c, which is what ranks the
 * menu/base-table registers like ROM); the party cursor's slots are g++
 * {delta, index, pfn} vtable entries.
 */

typedef unsigned char u8;
typedef unsigned short u16;

extern "C" {
extern const unsigned char D_80151E38[144];
extern const unsigned char D_80152AE8[144];
extern const u8 D_80152B78[], D_8014A9E8[], D_80151EC8[], D_80152580[], D_80152098[];
extern char D_80138CAC[], D_8014AB14[], D_8014AB24[], D_80160BC0[], D_80138E58[];
extern u16 D_8015470C[];
extern signed char D_80140160[];
}

/* Window base: method table at +0x4C; its destructor reinstalls the root table. */
struct Window {
    char pad0[0x4C];
    const void *vtable;
    ~Window() { vtable = D_80151E38; }
};

extern "C" {
void *func_800C9E00(void);
void *func_800953C0(void *self);
void func_8009D7EC(void *menu, void *party, void *layout, s32 value);
void func_80041434(s32 value);
s32 func_800957C0(void *object, void *output, s32 modal, void *history, s32 event);
void func_80091D20(void *cursor, u32 slot);
s32 func_800918B0(void *cursor);
s32 func_800D1488(void *party, u8 row);
void func_800D1344(void *party, u8 row);
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);
char *func_80083D04(char *dst, char *src);
s32 func_800D1604(void *party, s32 row);
void func_8009D610(void *obj, char *text, void *rect, void *position);
u8 func_800D163C(void *party);
void func_800D11C0(void *party);
u8 func_801E9F70(s32 flag);
void func_801F212C(u16 id, u8 flag);
s32 func_801EF340(s32 flag, s32 arg1);
void func_801E9F90(s32 flag, u8 value);
s32 func_8006D44C(s32 a, s32 b);
void func_80045A24(s32 sound);
void *func_80097AB0(void *self);
void func_80097BE0(void *obj, void **buf, s32 mode, const void *desc, s32 flag);
s32 func_8009A2E8(void *);
void func_800951D0(void *self, s32 flags);
}

/* Party record view (TU-local name): up to three member pointers at +0x28. */
struct PartyView_80047AF4 {
    char pad0[0x28];
    void *members[3];
};

typedef struct { short delta; short index; void (*open)(void *self); } OpenSlot;
typedef struct { short delta; short index; s32 (*current)(void *self); } IndexSlot;
typedef struct {
    char pad0[0x8];
    OpenSlot open_08;
    char pad10[0x48 - 0x10];
    IndexSlot current_48;
} CursorVtbl;

/* Cursor owner (vtable at +0x0C). */
struct Cursor {
    s32 index;
    void *records;
    s32 done;
    CursorVtbl *vtbl;
    s32 busy;
    s32 mode;
    void open() { vtbl->open_08.open((char *)this + vtbl->open_08.delta); }
    s32 current() { return vtbl->current_48.current((char *)this + vtbl->current_48.delta); }
    s32 pick()
    {
        open();
        func_80091D20(this, current());
        busy = 1;
        s32 slot = func_800918B0(this);
        busy = 0;
        func_80091D20(this, 0xFFFFFFFF);
        return slot;
    }
};

struct Choice { s32 index; char pad4[0x1C]; };

/* Party command menu (0x60 bytes). */
struct PartyMenu : Window {
    char pad50[0x10];
    PartyMenu() { func_800953C0(this); vtable = D_80152B78; }
    ~PartyMenu() { vtable = D_80152B78; }
};

/* Choice cursor (vtable at +0x10 over the base record's index at +0x0C). */
struct CursorBase {
    char pad0[0xC];
    s32 index;
    CursorBase() { index = -1; }
};
struct ListCursor : CursorBase {
    const void *vtable;
};

/* Dialog window view (TU-local name; method table D_80152AE8). */
struct DialogView_80047AF4 : Window {
    char pad50[0xC];
    DialogView_80047AF4() { func_800953C0(this); vtable = D_80152AE8; }
    ~DialogView_80047AF4() { vtable = D_80152AE8; }
};

/* Yes/no dialog (0x110 bytes): the dialog plus its choice cursor at +0x5C. */
struct ChoiceDialog : DialogView_80047AF4 {
    ListCursor cursor;
    char pad70[0xA0];
};

/* List member at +0x60 of the first panel child, destroyed out of line. */
struct PanelList {
    char pad0[0x20];
    ~PanelList() { func_800951D0(this, 2); }
};
struct PanelChildC8 : Window {
    char pad50[0x10];
    PanelList list;
};
struct PanelChild148 : Window {
    char pad50[0x10C - 0x50];
    ~PanelChild148() { vtable = D_80152AE8; }
};
struct PanelChild254 : Window {
    char pad50[0x98 - 0x50];
    s32 (*filter_98)(void *);
    char pad9C[0x1B8 - 0x9C];
    ~PanelChild254() { vtable = D_80152098; }
};
/* 0x40C-byte party panel, built by func_80097AB0. */
struct PartyPanel : Window {
    char pad50[0x78];
    PanelChildC8 childC8;
    PanelChild148 child148;
    PanelChild254 child254;
    PartyPanel() { func_80097AB0(this); }
    void setFilter(s32 (*filter)(void *)) { child254.filter_98 = filter; }
    ~PartyPanel() { vtable = D_8014A9E8; }
};

extern "C" s32 func_80047AF4(Cursor *self, s32 mode)
{
    Choice choice;
    s32 result = -1;
    PartyView_80047AF4 *party = (PartyView_80047AF4 *)func_800C9E00();

    self->mode = mode;
    if (mode != 0) {
        /* Each pass builds a fresh menu; the pass that finishes returns from
           inside the menu's scope (both exits run its destructor). */
        for (;;) {
            s32 done = 0;
            PartyMenu menu;
            func_8009D7EC(&menu, party, D_80138CAC, 0);
            func_80041434(1);
            s32 ok = func_800957C0(&menu, &choice, 1, 0, 0);
            func_80041434(0);
            if (!ok) {
                result = 0;
                done = 1;
            } else {
                switch (choice.index) {
                case 0: {
                    s32 slot = self->pick();
                    if (slot >= 0) {
                        u8 row = slot;
                        if (func_800D1488(party, row) >= 0) {
                            func_800D1344(party, row);
                        }
                    }
                    break;
                }
                case 3: {
                    ChoiceDialog dlg;
                    char text[0x100];
                    s32 answer[2];
                    s32 any = 0;
                    dlg.cursor.vtable = D_80151EC8;
                    func_80083C90(text, func_80048480(0x460));
                    for (s32 i = 0; i < 5; i++) {
                        if (func_800D1604(party, i)) {
                            if (any) {
                                func_80083D04(text, func_80048480(0x517));
                            }
                            func_80083D04(text, func_80048480(D_8015470C[i]));
                            any = 1;
                        }
                    }
                    func_80083D04(text, func_80048480(0x461));
                    func_8009D610(&dlg, text, D_8014AB14, D_8014AB24);
                    bool cancelled = func_800957C0(&dlg, answer, 1, 0, 0) != 1;
                    if (cancelled) {
                        answer[0] = 0;
                    }
                    if (answer[0]) {
                        result = func_800D163C(party);
                        done = 1;
                        break;
                    }
                }
                    /* declined: same as leaving */
                case 2:
                    func_800D11C0(party);
                    result = 0;
                    done = 1;
                    break;
                }
            }
            if (done) {
                return result;
            }
        }
    }
    if (!func_801E9F70(0x2A)) {
        func_801F212C(0x2C8, 0);
        func_801EF340(D_80140160[4] == 1, 1);
        func_801E9F90(0x2A, 1);
    }
    for (;;) {
        s32 r = func_8006D44C(3, -1);
        if (r == 0x34) {
            break;
        }
        if (r != 0x33) {
            continue;
        }
        while (func_800957C0(D_80160BC0, &choice, 1, 0, 0)) {
            switch (choice.index) {
            case 0:
                self->pick();
                break;
            case 1: {
                void *list[3];
                PartyPanel panel;
                s32 n = 0;
                void **members = party->members;
                for (s32 i = 0; i < 3; i++) {
                    if (members[i]) {
                        list[n++] = members[i];
                    }
                }
                for (; n < 3; n++) {
                    list[n] = 0;
                }
                func_80097BE0(&panel, list, 0x400, D_80138E58, 0);
                panel.setFilter(func_8009A2E8);
                func_800957C0(&panel, &choice, 1, 0, 0);
                break;
            }
            case 2:
                func_801F212C(0x2C8, 0);
                func_801EF340(D_80140160[4] == 1, 1);
                break;
            }
        }
    }
    func_80045A24(3);
    return result;
}
