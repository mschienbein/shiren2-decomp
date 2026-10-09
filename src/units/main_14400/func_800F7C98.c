#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { u8 field_00[0x20]; short field_20, field_22; s32 (*field_24)(void *); } InventoryTable;
typedef struct { s32 field_00; InventoryTable *table; u8 field_08[0x18]; } Inventory;
typedef struct { u8 field_00[0x8C]; Inventory inventory; u8 field_AC; } Shop;
typedef struct { u8 field_00[0x98]; short field_98, field_9A; void *(*field_9C)(void *); } PlayerTable;
typedef struct { u8 field_00[0x24]; PlayerTable *table; u8 field_28[0x5C]; u32 money; } Player;
typedef struct { u8 field_00[4]; u8 count; } Item;
typedef struct { void *container; void *item; } Pair;
typedef struct { u8 field_00[0x14]; } List;
typedef struct { u8 field_00[0xB0]; List items; } Order;
typedef struct { u32 kind; u8 field_04[0x1C]; } Event;
typedef struct { u8 field_00[0x4C]; const void *table; u8 field_50[0x18]; s32 field_68; void *field_6C; u8 field_70[0xA0]; } Window;
/* The next menu starts at 0x801408EC: this object occupies exactly 0x40C bytes. */
typedef struct { u8 field_00[0x2CC]; s32 field_2CC; u8 field_2D0[0x138]; short message_408; u8 field_40A[2]; } Menu;
extern Menu D_801404E0;
extern Player *D_801476B8;
extern u8 D_80138FF8[], D_80151EC8[];
extern const unsigned char D_80152AE8[144];
extern const unsigned char D_80151E38[144];
extern u8 D_8014AB2C[], D_8014AB3C[];
extern void func_800CDC2C(void *inventory, s8 mode);
extern void func_800CDCC8(void *inventory, s8 mode);
extern void func_80097B90(void *menu, void *owner, void *holder, s32 mode, void *desc, s32 flag);
extern s32 func_800EB820(Player *player);
extern s32 func_800EB8B0(s32 value);
extern void func_80099E50(Menu *menu);
extern void *func_800D8FB0(u32 bytes);
extern Order *func_800D9530(Order *self, s32 value);
extern s32 func_800957C0(void *window, Event *event, s32 mode, void *arg, s32 flag);
extern char *func_800A3B20(Shop *shop);
extern void func_800498E4(s32 id, ...);
extern s32 func_8009A038(Menu *menu);
extern Pair func_8009A054(Menu *menu, s32 index);
extern s32 func_800AE9AC(Item *item, s32 mode, s32 adjustment);
extern void func_800D05A4(List *list, Pair *selection);
extern s32 func_800CD278(void *inventory);
extern Window *func_800953C0(Window *window);
extern char *func_80048480(u16 id);
extern char *func_800AE674(void *item);
extern s32 func_8005EF08(char *out, const char *format, ...);
extern void func_8009D610(void *window, char *text, void *rect, void *position);
extern void func_80049BF0(s32 mode);
extern s32 func_80049CB4(s32 id, ...);
static inline s32 below(s32 value, s32 limit) { return value < limit; }
static inline void initWindow(Window *window) {
    func_800953C0(window);
    window->table = D_80152AE8;
}
Order *func_800F7C98(Shop *shop, s32 mode) {
    Event event;
    Pair selection;
    Window window;
    char text[200];
    void *inventory;
    Order *result;
    s32 adjustment;

    func_800CDC2C(&shop->inventory, 2);
    {
        s32 style = mode ? 0x40 : 0x80;
        func_80097B90(&D_801404E0, &shop->inventory, 0, style, D_80138FF8, 0);
    }
    if (mode)
        adjustment = func_800EB8B0(func_800EB820(D_801476B8));
    else
        adjustment = 0;
    inventory = D_801476B8->table->field_9C((u8 *)D_801476B8 + D_801476B8->table->field_98);
    {
        short message = 0x1E5;
        if (adjustment > 0) message = 0x1E7;
        D_801404E0.message_408 = message;
    }
retry:
    {
        Menu *menu = &D_801404E0;
        Order *order;
        func_80099E50(menu);
        result = 0;
        menu->field_2CC = 1;
        order = func_800D9530(func_800D8FB0(0xC8), mode);
        if (func_800957C0(menu, &event, 1, 0, 0)) {
            if (event.kind == 0x80000000) {
                func_800498E4(0x1DA, func_800A3B20(shop));
            } else {
                s32 count = func_8009A038(menu);
                u32 cost = 0;
                s32 quantity = 0;
                s32 i;
                for (i = 0; below(i, count); i++) {
                    Item *item;
                    selection = func_8009A054(&D_801404E0, i);
                    item = selection.item;
                    cost += func_800AE9AC(item, 1, adjustment);
                    quantity += item->count;
                    func_800D05A4(&order->items, &selection);
                }
                if (D_801476B8->money >= cost) {
                    if (func_800CD278(inventory) >= quantity) {
                        Window *dialog;
                        s32 failed;
                        initWindow(&window);
                        window.field_68 = -1;
                        window.field_6C = D_80151EC8;
                        if (count >= 2) {
                            func_8005EF08(text, func_80048480(0x1E2), count, cost);
                        } else {
                            Item *item;
                            selection = func_8009A054(&D_801404E0, 0);
                            item = selection.item;
                            func_8005EF08(text, func_80048480(0x1E1), func_800AE674(item), cost);
                        }
                        dialog = &window;
                        func_8009D610(dialog, text, D_8014AB2C, D_8014AB3C);
                        failed = func_800957C0(dialog, &event, 1, 0, 0) != 1;
                        if (failed) event.kind = 0;
                        if (!event.kind) {
                            dialog->table = D_80151E38;
                            goto retry;
                        }
                        if (!shop->field_AC && adjustment > 0)
                            func_800498E4(0x1DE, func_800A3B20(shop));
                        else
                            func_800498E4(0x1DD, func_800A3B20(shop));
                        result = order;
                        window.table = D_80151E38;
                    } else {
                        func_800498E4(0x1DC, func_800A3B20(shop));
                    }
                } else {
                    func_800498E4(0x1DB, func_800A3B20(shop));
                }
            }
        } else {
            Inventory *items = &shop->inventory;
            if (!items->table->field_24((u8 *)items + items->table->field_20))
                func_800498E4(0x1DA, func_800A3B20(shop));
            else
                func_800498E4(0x1DF, func_800A3B20(shop));
        }
    }
    func_80049BF0(0);
    func_80049CB4(2);
    func_800CDCC8(&shop->inventory, 2);
    return result;
}
