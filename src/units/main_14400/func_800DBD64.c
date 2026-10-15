#include "common.h"
#include "item_records.h"

/*
 * Item transfer command of the item-list family (siblings func_800DBBCC and
 * func_800DC054). The source link at +0x08 must release its item (the list's
 * +0x60 slot returns exactly 1) and the destination link at +0x10 must release
 * its own item. On success both items are re-registered (func_800DAD20), the
 * moved item receives message 10, and on acceptance the destination slot is
 * cleared (func_800D02AC). A refused release whose item is kind 9 closes the
 * menu D_80140160 (func_80094DA0). Every path returns 0.
 *
 * Codegen notes: see the ODD_C and FAKEMATCH comments in the body.
 */

typedef struct ItemLink {
    ItemList *list;
    Item *item;
} ItemLink;

/* Partial view: links at +0x08 and +0x10 (constructor func_800DBD00). */
typedef struct TransferCommand {
    unsigned char pad00[8];
    ItemLink source;
    ItemLink destination;
} TransferCommand;

/* Message 10 (0x20 bytes on the stack); the handler reads +4 and +8. */
typedef struct TransferMessage {
    ItemMessage base;
    void *sender;
    Item *other;
    unsigned char pad0C[0x14];
} TransferMessage;

extern void *D_801476B8;
extern unsigned char D_80140160[];
void func_800DAD20(void *command, void *item);
void func_800DAD80(void *command);
void *func_800D02AC(void *link);
void func_80094DA0(void *menu);

static inline s32 can_release(ItemList *list, Item *item)
{
    return list->methods->can_release64((char *)list + list->methods->delta60, item, 1);
}

static inline void report_refusal(Item *item)
{
    if (item->field00 == 9) {
        func_80094DA0(D_80140160);
    }
}

s32 func_800DBD64(TransferCommand *command)
{
    ItemLink *link = &command->destination;
    Item *item = command->source.item;
    Item *other = link->item;
    Item *pending;
    TransferMessage message;
    TransferMessage *event;
    s32 failed;

    failed = can_release(command->source.list, item) != 1;
    pending = item;
    /* ODD_C: a breakable block. The break really skips the refusal report and the
       early return when the source list released its item. Its NOTE_INSN_LOOP_END
       stops GCC 2.8.1 CSE (both passes) from following the first test into the
       continuation, so the second release and its report start a fresh CSE block
       that never records `pending = item`: the second report keeps the copy (s3,
       loaded at 0x800DBE78) while the success path keeps `item` (s0). The copy sits
       before the block and reorg moves it into the first beqz delay slot
       (0x800DBDB8). The plain `if (failed) { ...; return 0; }` merges both reports
       (75 words/320 bytes, wave15 d64_plain, rerun here as dbd64_base_plain). */
    do {
        if (!failed) {
            break;
        }
        report_refusal(item);
        return 0;
    } while (0);
    if (can_release(command->destination.list, link->item)) {
        func_800DAD20(command, other);
        func_800DAD20(command, item);
        message.base.kind = 10;
        event = &message;
        event->other = other;
        event->sender = D_801476B8;
        if (item->field08->handle3C((char *)item + item->field08->delta38, &event->base)) {
            func_800D02AC(link);
        } else {
            report_refusal(item);
        }
        func_800DAD80(command);
        return 0;
    }
    report_refusal(pending);
    /* FAKEMATCH: dead store with no semantic effect (flow deletes it; the ROM has no
       instruction for it). It moves item's REGNO_LAST_UID after pending's, so
       cse.c make_regs_eqv keeps `item` canonical in the entry block and the first
       report still reads s0 (0x800DBDBC); without it `pending` becomes canonical
       there and both reports merge: 69 words/332 bytes (opus-07 run
       dbd64_s5_cb_dst_noclear). A dead copy-back `pending = item;` or
       `item = pending = 0;` match equally (same mechanism, also dead). No form
       without a reference to `item` after this report matched; see
       opus-07/result.json. This is not a recovered original shape. */
    item = 0;
    return 0;
}
