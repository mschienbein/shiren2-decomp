/* OpenAI gpt-6-astra: native interpreter-local collection lifetime.
 * g++ emits the ROM's __tcf_0 and empty global init hook; neither is handwritten.
 * Generated-symbol credit and storage placement require harness support. */
extern "C" {
#include "types.h"

/* The facade receivers below agree with the staging3 definitions of
 * 4817C, 48204 and D1B04, including their unused receiver arguments. */
extern s32 func_800327C0(void *, const void *, ...);
extern u32 func_80032D70(void *);
extern void * func_80032D94(void *, void *, u32);
extern void func_80048130(void *, u8);
extern void func_8004817C(void *);
extern void func_800481B0(void *, s32);
extern void func_80048204(void *);
extern char * func_80048480(u16);
extern void func_8006D418(void);
extern void func_8006D438(void);
extern char * func_80083C90(void *, void *);
extern s32 func_800918B0(void *);
extern void func_80094B3C(void *, s32);
extern void func_80097B90(void *, void *, void *, s32, const void *, s32);
extern OvlPair func_80097F20(void *);
extern s32 func_8009A2B8(void *);
extern s32 func_8009A2C8(void *);
extern s32 func_8009A2F8(void *);
extern s32 func_8009A330(void *);
extern void func_8009ADE8(void *, s32, void *, s32);
extern u8 * func_8009C17C(void *);
extern void * func_800A0D80(void *, void *);
extern void func_800A0DA8(void *, void *);
extern s32 func_800A0E00(void *, s32 (*)(void *));
extern s32 func_800A0EF4(void *);
extern s32 func_800A100C(void *);
extern s32 func_800A1100(void *);
extern s32 func_800A1168(void *);
extern void func_800A124C(void *);
extern void func_800A1888(void *, void *);
extern s32 func_800A1890(void *);
extern s32 func_800A18AC(void *);
extern void func_800A19C0(void *, u32);
extern s32 func_800A1A64(void *);
extern s32 func_800A1BE4(void *);
extern void * func_800A1DA0(void *);
extern s32 func_800A1DAC(void *);
extern s32 func_800A1EE8(void *);
extern void func_800A202C(void *);
extern void * func_800A2154(void *, void *);
extern void * func_800A2184(void *, void *);
extern s32 func_800A2910(u8, u8, void *);
extern s32 func_800A8974(void *);
extern OvlActor * func_800A8CB0(s32);
extern char * func_800A9840(u8, u8);
extern u8 func_800A991C(u8, u8);
extern u8 func_800A9A78(u8);
extern s32 func_800A9BC0(u8, void *);
extern s32 func_800A9C1C(u8, void *);
extern void func_800A9C5C(s32);
extern s32 func_800AC1FC(void);
extern s32 func_800AC670(void *);
extern void func_800ACD34(void *);
extern void func_800AE518(void *, void *, s32, s32);
extern void func_800B6F58(void *);
extern void func_800B7340(void *);
extern void func_800B74F4(s32);
extern void func_800B7604(void *, s32);
extern u8 func_800C57CC(void *, s32);
extern void func_800C78F8(s32);
extern void func_800C799C(void);
extern void func_800C7A40(void);
extern void func_800C7A9C(void);
extern void func_800C92BC(void *);
extern void func_800C9800(s32);
extern void func_800C9820(s32, s32);
extern void func_800C9870(void);
extern u8 * func_800C9E00(void);
extern OvlSaveRecord * func_800C9E10(void);
extern void func_800CABD8(void *);
extern void func_800CAD04(void *, void *, void *);
extern void func_800CB154(u32, void *);
extern void func_800CD364(void *, void *);
extern void * func_800D0180(void *);
extern s32 func_800D0FF0(void);
extern s32 func_800D1054(s32);
extern void func_800D10C4(void *);
extern s32 func_800D1A24(void *);
extern s32 func_800D1B04(void *);
extern void func_800D4CE4(void *, s8);
extern void func_800D4D8C(void *);
extern void func_800D4FB4(void *, u32);
extern void func_800D4FF0(void *);
extern s32 func_800D500C(void *, s32);
extern s32 func_800D800C(void);
extern void func_800D84C0(void *);
extern s32 func_800D8514(void *);
extern s32 func_800D8618(void *);
extern s32 func_800D86F4(void *);
extern void func_800D88F0(void *);
extern void * func_800D89C0(void *, void *);
extern void func_800D89F0(void *, void *);
extern char * func_800D8B80(void);
extern void * func_800EB9FC(void *);
extern s32 func_800ECA38(void *, u32, s32);
extern s32 func_8010BC2C(void *, u8);
extern void func_800265E0(void *, s32);
extern s32 func_80049CB4(s32, ...);
extern s32 func_800957C0(void *, void *, s32, void *, s32);

extern void func_801E6464_138384(s32);
extern void func_801E7BF8_139B18(void);
extern void func_801E85FC_13A51C(void);
extern void func_801E8618_13A538(void);
extern void func_801E8728_13A648(s32);
extern u8 func_801E89EC_13A90C(void);
extern u8 func_801E8CF4_13AC14(u8, u8);
extern s32 func_801E8D0C_13AC2C(u16, u16);
extern void func_801E8F20_13AE40(s32);
extern u32 func_801E8F90_13AEB0(u8 *, u8);
extern void func_801E9064_13AF84(OvlSelection *, u8 *, u8 *, u8);
extern void func_801E92C0_13B1E0(OvlSelection *, u8);
extern void func_801E9588_13B4A8(u8 *);
extern void func_801E981C_13B73C(void);
extern s32 func_801E9A38_13B958(void);
extern void func_801E9A90_13B9B0(void);
extern void func_801E9AFC_13BA1C(u8, u8);
/* ROM callers pass constants 1/20; the provider tests signed low-half count. */
extern void func_801E9B64_13BA84(s16);
extern u8 func_801E9F70_13BE90(s32);
extern void func_801E9F90_13BEB0(s32, u8);
extern s32 func_801EA990_13C8B0(s32, OvlItem *, s32);
extern void func_801EB020_13CF40(s32, s32);
extern s32 func_801EB0B4_13CFD4(s32);
extern s32 func_801EB11C_13D03C(s32);
extern s32 func_801EB1A0_13D0C0(s32);
extern void func_801EB208_13D128(void);
extern void func_801EB348_13D268(const void *);
extern void func_801EB394_13D2B4(void);
extern void func_801EB6A0_13D5C0(u8);
extern void func_801EEA38_140958(void);
extern void func_801EEAEC_140A0C(void);
extern void *func_801F1510_143430(OvlSpawnRecord *, s32, s32);
extern void func_801F2254_144174(u16, s32);
extern s32 func_801F2888_1447A8(u8);
extern void func_801F3238_145158(void);
extern void func_801F32A8_1451C8(void);
extern void func_801F4A8C_1469AC(void);
extern void func_801F4FE0_146F00(s32);

static inline OvlCollection *player_collection(OvlActor *player)
{
    OvlActorTable *table = player->table24;
    return table->collection9C((u8 *)player + table->delta98);
}

/* Resolve the player's collection through its real +98/+9C slot, then
 * initialize the inventory menu from it (801F1174..11AC). Caption assignment
 * belongs to the subsequent display step, not this initialization operation. */
/* D_80138F90 is pair 2 inside D_80138F80[3], not a separate object. */
static inline void initialize_player_item_menu(MenuPanel &menu, OvlActor *player)
{
    func_80097B90(&menu, player_collection(player), 0, 0x1000, &D_80138F80[2], 0);
}

/* The saved-time action formats the record's seconds and displays that text.
 * Both calls and the 100-byte caller-owned buffer are present in the ROM. */
static inline void display_saved_time(const OvlSaveRecord &save, char (&text)[100])
{
    func_800CB154(save.seconds18, text);
    func_801EB348_13D268(text);
}

/* ODD_C: these base/derived vtable transitions model temporary pool lifetime.
 * Initialization at 801F0BA0..0BD0 precedes reading the indexed data pointer;
 * Destruction at 801F0DE4..0E00 restores the base table. Removing its first,
 * overwritten table store (f7f0-06) emits 7516 bytes / 1769 differing words,
 * versus 7536 / 1776 with the observed transition (f7f0-05). Raw score falls
 * because branch offsets move; the ROM's destructor instructions disappear. */
static inline void open_pool(OvlPool *pool, u8 index)
{
    pool->vtable = &D_80149DB8;
    func_8013687C(&pool->pool);
    pool->vtable = &D_80149DA8;
    pool->pool = D_80147FD0[index];
}

static inline void close_pool(OvlPool *pool)
{
    pool->vtable = &D_80149DA8;
    pool->pool = 0;
    pool->vtable = &D_80149DB8;
}

}
/* Real cursor constructors; shared ItemCursor and the value cursor are12 bytes. */
struct ScriptItemCursor : ItemCursor {
    ScriptItemCursor() { func_800A0D80(this, 0); }
};
struct ScriptValueCursor : OvlValueCursor {
    ScriptValueCursor() { func_800A1DA0(this); }
};
/* ROM801EF834..848 tests/sets CE4 with no call, object store or address.
 * The free interval between the cursors is6F5C..6F70. Sizes17..20 compile
 * identically;20 is the maximal conservative declaration, not a unique size.
 * The whole-ROM scan found no literal or known-base CPU reference into it;
 * five low-half matches all address resident80036F60/64, not this interval.
 * Main explicitly approved opaque fields for this evidenced static after
 * reviewing static-interval-scan.json. Its role/field widths remain unknown. */
struct StaticObject801F6F5C {
    unsigned char opaque[20];
    StaticObject801F6F5C() {}
};
/* CF0's cold path only sets its guard (801EF8CC..8DC). The holder is8 bytes:
 * A1888 writes its list at+0; A18AC reads+0 and writes the created item at+4.
 * All CPU references are here. The raw801F7056 word at ROM67EEC0 is inside
 * the reviewed Yay0 stream67C9D0..68D557, not pointer/relocation data. */
struct ScriptListHolder : OvlListHolder {
    ScriptListHolder() {}
};


extern "C" {
void func_801EF7F0_141710(s32 opcode)
{
    /* ORIGINAL BUG: case0x77 with a nonzero operand and a nonnull save
     * takes801F1300 ->801F1334, skipping memset at801F1308 and CAD04 at
     * 801F131C. The call at801F1334 passes this buffer at sp+18 via801F1338;
     * 801EB348 ->801ECF5C reads its unset bytes. The named case is approved
     * by scratch/omp/rulings/ORIGINAL-BUG-UNINIT.md. Defined alternatives:
     * entry aggregate zero-init adds16 bytes and emits unresolved memset;
     * a NUL store on the nonzero branch adds4 bytes and differs251 words.
     * Whether shipped scripts reach this path is not established. */
    char auxiliary[100];
    char text[100];
    u8 *grid;

    static ScriptItemCursor items;
    static StaticObject801F6F5C object6F5C;
    static ScriptValueCursor values;
    static OvlPairCollection collection;
    static ScriptListHolder holder;
    grid = func_800C9E00();
    func_800265E0(text, sizeof(text));
    switch (opcode) {
    case 1:
        if (!D_801F51E8_147108->unk_996) {
            func_800C9820(D_80142F24.index, D_80142F24.count);
            func_800C9870();
        }
        break;
    case 2:
        D_801F51E8_147108->field9F0 = 2;
        break;
    case 3:
        func_800C9800(1);
        func_80094B3C(D_80140160, 2);
        func_800C9870();
        break;
    case 4:
        D_801F51E8_147108->field9F0 = 1;
        break;
    case 7:
        func_800D4CE4(D_80147F90, 0);
        break;
    case 8:
        func_800D4D8C(D_80147F90);
        func_800C799C();
        break;
    case 9: {
        s32 i;
        u8 mask;
        func_800D10C4(func_800C9E00());
        mask = func_800D1B04(func_800C9E00());
        if (mask) {
            func_801E9F90_13BEB0(0x15, 1);
            if (mask == 0x1F) func_801E9F90_13BEB0(0x16, 1);
            for (i = 4; i != -1; i--) func_801E9F90_13BEB0(i + 0x10, (mask >> i) & 1);
        }
        break;
    }
    case 0xA: {
        s32 value = D_801F51E8_147108->args[1].word;
        s32 row = value / 3;
        s32 column = value % 3;
        func_800D4FB4(D_80147FB8, row);
        func_801E8CF4_13AC14(4, func_800D500C(D_80147FB8, column));
        func_800D4FF0(D_80147FB8);
        break;
    }
    case 0x7D:
        func_800C7A40();
        break;
    case 0xC:
        func_800C78F8(5);
        break;
    case 0xE:
        func_800C7A9C();
        func_80049CB4(2);
        break;
    case 0x11:
        func_801E8CF4_13AC14(4, func_800D1A24(grid));
        break;
    case 0x12:
        func_801E8CF4_13AC14(4, func_801E9A38_13B958());
        break;
    case 0x1A: {
        s32 total = func_800D1A24(grid);
        func_801E8CF4_13AC14(4, total - func_801E9A38_13B958());
        break;
    }
    case 0x14: {
        s32 saved, step;
        func_801E9B64_13BA84(1);
        if (D_801F5228_147148[2] >= 0xA6) break;
        if (D_801F5228_147148[2] < 0x50) break;
        if (!func_801E9A38_13B958()) break;
        saved = D_801F5228_147148[0];
        step = func_800C57CC(D_80147620, 1);
        saved++;
        if (step == 1) step = func_800C57CC(D_80147620, 1) + 1;
        if (step == 2) step = func_800C57CC(D_80147620, 1) + 2;
        if (step == 3) step = 4;
        step += saved;
        func_801E8CF4_13AC14(0, step);
        if (step >= 5) {
            s32 count = 4;
            func_800265E0(D_801F5218_147138, 16);
            func_800265E0(D_801F5214_147134, 4);
            func_800265E0(D_801F5210_147130, 4);
            if (func_801E9A38_13B958() < 8) count = 1;
            else if (func_801E9A38_13B958() < 11) count = 2;
            else if (func_801E9A38_13B958() < 14) count = 3;
            func_801E9064_13AF84(D_801F5218_147138, D_801F5214_147134, D_801F5210_147130, count);
        }
        break;
    }
    case 0xF:
        func_800A9C5C(0);
        break;
    case 0x16:
        func_801E92C0_13B1E0(D_801F5218_147138, 4);
        break;
    case 0x17: {
        switch (D_801F51E8_147108->args[1].word) {
        case 0: case 1: case 2: case 3: {
            s32 i;
            OvlSelection *record = &D_801F5218_147138[D_801F51E8_147108->args[1].word];
            for (i = 4; i != -1; i--) func_801E9F90_13BEB0(i + 16, 0);
            if (record->mask) {
                func_801E9F90_13BEB0(20, record->active);
                func_801E8CF4_13AC14(4, record->column + record->row * 4 + 1);
                for (i = 3; i != -1; i--) { if ((record->mask >> i) & 1) func_801E9F90_13BEB0(i + 16, 1); else func_801E9F90_13BEB0(i + 16, 0); }
            } else func_801E8CF4_13AC14(4, 0);
            break;
        }
        case 4: case 5:
            func_801E8CF4_13AC14(4, func_801E8F90_13AEB0(D_801F5214_147134, 1));
            break;
        case 6: {
            s32 i;
            for (i = 3; i != -1; i--) func_801E9F90_13BEB0(i + 16, D_801F5214_147134[i]);
            break;
        }
        }
        break;
    }
    case 0x33:
        func_801E9588_13B4A8(D_801F5210_147130);
        break;
    case 0x15: {
        s32 i;
        func_801E9F90_13BEB0(16, 1);
        for (i = 0; i < 5; i++) {
            s32 j;
            for (j = 0; ; j++) {
                s32 more = j < 4;
                if (!more) break;
                s32 index = j + i * 4;
                if (grid[index] != 100) {
                    func_801E9F90_13BEB0(16, 0);
                    break;
                }
            }
        }
        break;
    }
    case 0x34:
        func_801E8CF4_13AC14(4, D_801F5210_147130[D_801F51E8_147108->args[1].word]);
        if (D_801F5210_147130[D_801F51E8_147108->args[1].word])
            func_801E8CF4_13AC14(5, (D_801F5210_147130[D_801F51E8_147108->args[1].word] - 1) % 4 + 1);
        break;
    case 0x18: {
        s32 i;
        s32 index;
        /* ROM 801EFEEC..801EFF00 snapshots all three byte operands before
         * either grid-writing loop. Reloading through the byte aliases inside
         * the loops changes both that dataflow and the generated code. */
        s32 count = D_801F5228_147148[7];
        s32 row = D_801F5228_147148[5];
        s32 value = D_801F5228_147148[6];
        for (i = 0; i < count; i++) {
            index = i + row * 4;
            grid[index] = value;
        }
        for (; i < 4; i++) {
            index = i + row * 4;
            grid[index] = 0;
        }
        for (i = 4; i != -1; i--) func_801E9AFC_13BA1C(i, 0);
        func_801E9B64_13BA84(20);
        func_801E9A90_13B9B0();
        break;
    }
    case 0x19: {
        s32 i;
        for (i = 0; i < D_801F51E8_147108->args[1].word; i++) grid[i / 4 * 4 + i % 4] = 50;
        for (; i < 20; i++) grid[i / 4 * 4 + i % 4] = 0;
        for (i = 4; i != -1; i--) func_801E9AFC_13BA1C(i, 0);
        func_801E9B64_13BA84(20);
        /* fall through */
    }
    case 0x13:
        func_801E9A90_13B9B0();
        break;
    case 0x10: {
        s32 row_offset = D_801F5228_147148[5] * 4;
        s32 column = D_801F5228_147148[7];
        func_801E8CF4_13AC14(4, grid[column + row_offset]);
        break;
    }
    case 0x20:
        func_800B6F58(D_80142B10);
        func_800C92BC(D_801476B8);
        break;
    case 0x21: {
        OvlScene *scene = D_80142B10;
        scene->table08->call34((u8 *)scene + scene->table08->delta30);
        break;
    }
    case 0x22: {
        OvlScene *scene = D_80142B10;
        scene->table08->call3C((u8 *)scene + scene->table08->delta38);
        func_80049CB4(0xDB);
        break;
    }
    case 0x23:
        func_800B7340(D_80142B10);
        func_80049CB4(0xDB);
        break;
    case 5: func_801EB020_13CF40(D_801F51E8_147108->args[1].word, 1); break;
    case 6: func_801EB020_13CF40(D_801F51E8_147108->args[1].word, 0); break;
    case 0x24: func_801E85FC_13A51C(); break;
    case 0x25: func_801E9F90_13BEB0(16, func_801E89EC_13A90C()); break;
    case 0x26: func_801E8618_13A538(); break;
    case 0x27: func_801F2254_144174(D_801F51E8_147108->args[1].half[1], 0); break;
    case 0x28: {
        u8 count = func_800A9A78(D_801F51E8_147108->args[1].byte[3] & 31);
        OvlCollection *collection;
        func_801E9F90_13BEB0(16, 0);
        func_801E8CF4_13AC14(4, count);
        collection = player_collection(D_801476B8);
        if (count < (u32)collection->table04->count24((u8 *)collection + collection->table04->delta20))
            func_801E9F90_13BEB0(16, 1);
        break;
    }
    case 0x29: case 0x2A: {
        OvlSpawnRecord record;
        OvlBPos position;
        u8 valid;
        s32 value, id;
        valid = 1;
        position.x = D_801476B8->x;
        position.y = D_801476B8->y;
        func_800265E0(&record, 0x16);
        func_801E9F90_13BEB0(23, 0);
        if (D_801F5228_147148[2] >= 0xFD) {
            if (opcode != 0x29) break;
        } else {
            s32 failed;
            if (opcode == 0x29) {
                failed = func_800A9BC0(D_801F51E8_147108->args[1].byte[3], &valid) != 1;
                if (failed) break;
            } else {
                failed = func_800A9C1C(D_801F51E8_147108->args[1].byte[3], &valid) != 1;
                if (failed) break;
            }
        }
        value = func_801F2888_1447A8(D_801F51E8_147108->args[1].byte[3]);
        if (!value) break;
        id = D_801F51E8_147108->args[1].byte[3];
        record.value0C = value;
        record.coordinates.bytes.y = position.y;
        record.coordinates.bytes.x = position.x;
        record.id00 = id;
        if (func_801F1510_143430(&record, valid, 0)) func_801E9F90_13BEB0(23, 1);
        break;
    }
    case 0x2B: D_801F51E8_147108->field9F6 = D_801F51E8_147108->args[1].byte[3]; break;
    case 0x2C: D_801F51E8_147108->field9F7 = D_801F51E8_147108->args[1].byte[3]; break;
    case 0x2D: func_801F4A8C_1469AC(); break;
    case 0x2E: func_801E8728_13A648(D_801F51E8_147108->args[1].word); break;
    case 0x2F: func_801E9F90_13BEB0(16, func_800D0FF0()); break;
    case 0x30: func_800D7444(D_801F51E8_147108->args[1].half[1]); break;
    case 0xD: case 0x32:
        func_800C799C();
        func_80049CB4(2);
        break;
    case 0x31: func_801E9F90_13BEB0(0x27, D_801480CE != 0); break;
    case 0x35: func_800A0DA8(&items, 0); break;
    case 0x36:
        switch (D_801F51E8_147108->args[1].word) {
        case 0: func_801E9F90_13BEB0(16, func_800A0E00(&items, func_8009A2B8)); break;
        case 1: func_801E9F90_13BEB0(16, func_800A0E00(&items, func_8009A2C8)); break;
        case 2: func_801E9F90_13BEB0(16, func_800A0E00(&items, func_8009A2F8)); break;
        default: func_801E9F90_13BEB0(16, func_800A0E00(&items, func_8009A330)); break;
        }
        break;
    case 0x3A: func_801E8CF4_13AC14(4, func_800A0EF4(&items)); break;
    case 0x3C: func_801E8CF4_13AC14(4, func_800A1100(&items)); break;
    case 0x3D: func_801E8CF4_13AC14(4, func_800A100C(&items)); break;
    case 0x37: func_801E8CF4_13AC14(4, func_800A1168(&items)); break;
    case 0x38:
        if (func_800A1218(&items) == 3) func_801E8CF4_13AC14(7, 1);
        else func_801E8CF4_13AC14(7, 2);
        break;
    case 0x39: func_800A124C(&items); break;
    case 0x3E: {
        /* ORIGINAL BUG: scoped user ruling ORIGINAL-BUG-UNINIT.md, opcode
         * 0x3E. Reset 0x35 -> successful 0x3C kind-6 selection -> 0x3E mode
         * 1/2 reaches A2910's no-store return at 800A2928/800A292C. The
         * ignored result leaves names unset for the reads at 801F0580 and
         * 801F05AC. Shipped-script reachability is not established.
         * Defined-value alternatives: conditional explicit zero stores
         * differ by1096 words; unconditional stores by1097 (both frame344,
         * body7456). Aggregate zero-init adds20B and an unbound memset. */
        u16 names[2];
        OvlItem *item = items.item;
        if (D_801F51E8_147108->args[1].word != 0)
            func_800A2910(item->field00, func_8010BC2C(item, items.index.byte[3]), names);
        switch (D_801F51E8_147108->args[1].word) {
        case 0: func_80083C90(text, func_800AE674(item)); break;
        /* ORIGINAL BUG: 801F0580 reads sp+E8 after the 0x35 -> 0x3C kind-6
         * -> 0x3E mode1 path and A2910's 800A2928/800A292C no-store return.
         * The scoped ruling permits this; zero-store alternatives miss
         * by1096/1097 words, as recorded at the names declaration. */
        case 1: func_800327C0(text, D_801F60F4_148014, func_80048480(names[0])); break;
        /* ORIGINAL BUG: 801F05AC reads sp+EA on the same 0x35 -> 0x3C
         * kind-6 -> 0x3E mode2 path, after 800A2928/800A292C stored nothing.
         * The scoped ruling and1096/1097-word defined-value alternatives
         * are the same as for names[0], not a general uninitialised waiver. */
        case 2: func_80083C90(text, func_80048480(names[1])); break;
        }
        func_801EB348_13D268(text);
        break;
    }
    case 0x3F:
        func_800B74F4(D_801F51E8_147108->args[1].word);
        func_80049CB4(2);
        break;
    case 0x40: {
        OvlBPos position;
        position.x = 10; position.y = 10;
        func_800B7604(&position, D_801F51E8_147108->args[1].word);
        func_80049CB4(2);
        break;
    }
    case 0x41: {
        /* ROM uses sp+E8. Every capacity14..30 compiles identically with
         * that formatter slot and frame344;30 is the conservative largest.
         * GCC function.c:910 retains logical size, not aligned physical size;
         * combine_temp_slots joins the22-byte record and8-byte position into
         * logical30/physical32 bytes, so31/32 cannot reuse this slot.
         * @%ds needs at most14 bytes including NUL. Other scratch lifetimes
         * are independent; the original declaration's exact size is unknown. */
        char formatted[30];
        s32 length;
        func_801EB6A0_13D5C0(1);
        func_800327C0(formatted, func_80048480(0x4FA), D_801F51E8_147108->args[1].word);
        length = func_80032D70(formatted);
        /* ODD_C: independently reviewed in interpreter-review-opus/review.json
         * checks.6; N64 pointers and u32 are32 bits.
         * cp/typeck.c:3184-3189 normalizes pointer+integer to pointer-first.
         * The ROM instead adds len11E to the state address at801F0650, then
         * adds text_126's0x126 offset in the call delay slot at801F0658.
         * Measured alternatives: typed cursor7, named index2, whole-object
         * u8* cursor4 differing words; ordinary typed indexing also differs2.
         * Integer byte arithmetic is plausible for this length-tracked
         * command stream: append the variable-length formatted @%ds token
         * without its NUL, then advance len11E separately. This does not
         * establish the original spelling; it is a measured reconstruction,
         * not natural source. Obj* storage and memcpy's void* contract stay
         * unchanged, and no fictitious object or wider array is introduced. */
        u32 write_address = D_801F51E8_147108->len11E
            + reinterpret_cast<u32>(D_801F51E8_147108);
        func_80032D94(reinterpret_cast<void *>(write_address + 0x126), formatted, length);
        D_801F51E8_147108->len11E += length;
        break;
    }
    case 0x42:
        func_801F4FE0_146F00(D_801F51E8_147108->args[1].word != 0);
        func_80049CB4(2);
        break;
    case 0x46:
        func_801E8CF4_13AC14(4, func_800A1A64(D_80142930));
        func_801EB394_13D2B4();
        break;
    case 0x47:
        func_801E8CF4_13AC14(4, func_800A1BE4(D_80142930));
        func_801EB394_13D2B4();
        break;
    case 0x49:
        func_800A1888(&holder, player_collection(D_801476B8));
        func_801E8CF4_13AC14(4, func_800A18AC(&holder));
        break;
    case 0x4A: func_800A1890(&holder); break;
    case 0x4B: D_801F51E8_147108->field9FB = D_801F51E8_147108->args[1].byte[3]; break;
    case 0x4C: D_801F51E8_147108->unk_9F8 = 1; break;
    case 0x48:
        if (D_801F51E8_147108->args[1].word) func_80048130(D_80140100, 1);
        else func_80048130(D_80140100, 0);
        func_800918B0(D_80140100);
        func_8004817C(D_80140100);
        break;
    case 0x4D: func_801E6464_138384(D_801F51E8_147108->args[1].word); break;
    case 0x4E:
        if (D_801F51E8_147108->args[1].word) func_801F3238_145158();
        else func_801F32A8_1451C8();
        break;
    case 0x4F:
        if (D_801F51E8_147108->args[1].word) func_801E8CF4_13AC14(4, func_800A1DAC(&values));
        else func_801E8CF4_13AC14(4, func_800A1EE8(&values));
        break;
    case 0x50:
        if (D_801F51E8_147108->args[1].word) func_800A2154(&values, text);
        else func_800A2184(&values, text);
        func_801EB348_13D268(text);
        break;
    case 0x51: {
        s32 value;
        if (D_801F51E8_147108->args[1].word) value = values.object->value0C;
        else value = values.value;
        func_801E8CF4_13AC14(4, value);
        func_800A202C(&values);
        func_801E981C_13B73C();
        break;
    }
    case 0x5F: {
        /* Same name-menu result contract as reviewed func_800C9E30: path[2],
         * with an8-byte original slot there. Main approved this shared extent.
         * Table80152800+74 actually calls9ADD8 (always-null child), so only
         * the first word is written; two words does not imply depth two. */
        s32 selected[2];
        func_801E9F90_13BEB0(16, 0);
        func_801E8CF4_13AC14(4, 0); func_801E8CF4_13AC14(5, 0);
        func_801E8CF4_13AC14(6, 0); func_801E8CF4_13AC14(7, 0);
        func_8009ADE8(D_80141AD0, 4, 0, 0);
        if (func_800957C0(D_80141AD0, selected, 1, 0, 0)) {
            u8 *values = func_8009C17C(D_80141AD0);
            func_801E8CF4_13AC14(4, values[0]); func_801E8CF4_13AC14(5, values[1]);
            func_801E8CF4_13AC14(6, values[2]); func_801E8CF4_13AC14(7, values[3]);
            func_801E9F90_13BEB0(16, 1);
        }
        break;
    }
    case 0x5E: func_801EB208_13D128(); break;
    case 0x52: func_801E8CF4_13AC14(4, D_801F5228_147148[9] % D_801F51E8_147108->args[1].word); break;
    case 0x53: {
        OvlActor *actor = func_800A8CB0((u8)func_801E8D0C_13AC2C(D_801F51E8_147108->args[1].half[1], 0));
        if (!func_800A8974(actor)) actor->flags1C |= 1;
        break;
    }
    case 0x54:
        if (D_801F51E8_147108->args[1].word) func_801F3238_145158();
        else func_801F32A8_1451C8();
        break;
    case 0x55: {
        s32 index = func_801EB11C_13D03C(D_801F51E8_147108->args[1].word);
        if (index >= 0) {
            OvlItemDescriptor *desc = &D_80148780[index];
            func_80083C90(text, func_800A9840(desc->kind, desc->variant));
            func_801EB348_13D268(text);
            func_801E8CF4_13AC14(4, func_800A991C(desc->kind, desc->variant));
        }
        break;
    }
    case 0x56: {
        s32 index = func_801EB11C_13D03C(D_801F51E8_147108->args[1].word / 2);
        if (index >= 0) {
            OvlItem *item = func_800AFD78(func_800AF818(index + 4), D_801F51E8_147108->args[1].word % 2);
            if (item && !func_800AC670(item)) {
                func_800ACD34(item);
                func_80083C90(text, func_800AE674(item));
                func_801EB348_13D268(text);
            }
        }
        break;
    }
    case 0x57: {
        s32 i;
        s32 mask = 0;
        s32 index = func_801EB11C_13D03C(D_801F51E8_147108->args[1].word);
        if (index >= 0) {
            ItemPool *table = func_800AF818(index + 4);
            for (i = 0; ; i++) {
                OvlItem *item;
                if (i >= 2) break;
                item = func_800AFD78(table, i);
                if (item && !func_800AC670(item)) mask += 1 << i;
            }
            func_801E8CF4_13AC14(4, mask);
        }
        break;
    }
    case 0x58: func_801E8CF4_13AC14(5, func_801EB11C_13D03C(4)); break;
    case 0x59: {
        OvlPool pool;
        s32 i;
        s32 index;
        func_801E9F90_13BEB0(16, 0);
        index = func_801EB0B4_13CFD4(0);
        if (index == -1) break;
        open_pool(&pool, index);
        for (i = 0; ; i++) {
            OvlItem *item;
            if (i >= 2) break;
            item = func_800D561C(&pool, i);
            if (item) {
                func_801E9F90_13BEB0(16, 1);
                break;
            }
        }
        close_pool(&pool);
        break;
    }
    case 0x5C: {
        OvlPool pool;
        s32 i;
        s32 index = func_801EB0B4_13CFD4(0);
        s32 mask;
        if (index == -1) break;
        open_pool(&pool, index);
        mask = 0;
        for (i = 0; ; i++) {
            OvlItem *item;
            if (i >= 2) break;
            item = func_800D561C(&pool, i);
            if (item) mask += 1 << i;
        }
        func_801E8CF4_13AC14(4, mask);
        close_pool(&pool);
        break;
    }
    case 0x5B: {
        OvlPool pool;
        OvlItem *item;
        s32 index = func_801EB0B4_13CFD4(0);
        if (index == -1) break;
        open_pool(&pool, index);
        item = func_800D561C(&pool, D_801F51E8_147108->args[1].word);
        if (item) {
            func_80083C90(text, func_800AE674(item));
            func_801EB348_13D268(text);
        }
        close_pool(&pool);
        break;
    }
    case 0x5A: {
        OvlPool pool;
        s32 i;
        s32 index = func_801EB0B4_13CFD4(0);
        if (index == -1) break;
        open_pool(&pool, index);
        for (i = 0; ; i++) {
            OvlItem *item;
            if (i >= 2) break;
            item = func_800D55F0(&pool, i);
            if (item && !func_800AC670(item)) func_801EA990_13C8B0(23, item, 0);
        }
        close_pool(&pool);
        break;
    }
    case 0x60: func_800D84C0(&collection); break;
    case 0x61:
        if (D_801F51E8_147108->args[1].word) func_800D89C0(&collection, text);
        else func_800D89F0(&collection, text);
        func_801EB348_13D268(text);
        break;
    case 0x62: func_801E8CF4_13AC14(4, func_800D8514(&collection)); break;
    case 0x63: func_801E8CF4_13AC14(4, func_800D8618(&collection)); break;
    case 0x65: func_801E8CF4_13AC14(4, func_800D86F4(&collection)); break;
    case 0x66: func_800D88F0(&collection); break;
    case 0x67: {
        s32 count = 162 - func_800D800C();
        func_801E8CF4_13AC14(6, count / 100);
        func_801E8CF4_13AC14(7, count % 100);
        break;
    }
    case 0x68: {
        OvlSaveRecord *save = func_800C9E10();
        if (save) display_saved_time(*save, text);
        break;
    }
    case 0x5D: func_801E9F90_13BEB0(16, D_801F51E8_147108->unk_996); break;
    case 0x69:
        func_80083C90(text, func_800D8B80());
        func_801EB348_13D268(text);
        break;
    case 0x6A: {
        OvlSaveRecord *save = func_800C9E10();
        if (save) {
            func_800327C0(text, D_801F6100_148020, save->id00);
            func_801EB348_13D268(text);
        }
        break;
    }
    case 0x6B: func_800481B0(D_80140130, D_801F51E8_147108->args[1].word); break;
    case 0x6C: func_80048204(D_80140130); break;
    case 0x6D: func_801E8CF4_13AC14(4, func_800D800C()); break;
    case 0x6E:
    {
        Obj801F51E8 *state = D_801F51E8_147108;
        s32 busy;
        if (state->unk_996) break;
        busy = D_801F5D00_147C20 == 1;
        if (!busy) {
            s32 value = state->args[2].word + 45;
            func_801E7BF8_139B18();
            func_800ECA38(D_801476B8, value, D_801F51E8_147108->args[1].word != 0);
        }
        break;
    }
    case 0x6F: func_800A19C0(D_80142930, D_801476B8->value84); break;
    case 0x70: {
        s32 i;
        s32 count = 0;
        for (i = 2; i != -1; i--) if (func_800D5484(i)) count++;
        func_801E8CF4_13AC14(5, count);
        break;
    }
    case 0x72: {
        s32 index = func_801EB1A0_13D0C0(D_801F51E8_147108->args[1].word);
        if (index >= 0) {
            OvlItem *item = func_800AFD78(func_800AF818(index + 4), 2);
            if (item && !func_800AC670(item)) {
                func_80083C90(text, func_800AE674(item));
                func_801EB348_13D268(text);
            }
        }
        break;
    }
    case 0x71: {
        s32 index = func_801EB1A0_13D0C0(D_801F51E8_147108->args[1].word);
        if (index >= 0) {
            OvlItemDescriptor *desc = &D_80148780[index];
            func_80083C90(text, func_800A9840(desc->kind, desc->variant));
            func_801EB348_13D268(text);
            func_801E8CF4_13AC14(4, func_800A991C(desc->kind, desc->variant));
        }
        break;
    }
    case 0x73: {
        /* 957C0's driver reserves an eight-level path in D_80140234[8]
         * and D_80140210.entries[8]. Use its full result capacity here:
         * ROM801F11BC addresses sp+108..128. Smaller arrays alter temporary
         * slot reuse; this is not a claim that every item menu reaches depth8. */
        s32 selected[8];
        initialize_player_item_menu(D_801404E0, D_801476B8);
        OvlMenuView *menu = (OvlMenuView *)&D_801404E0;
        s32 failed;
        menu->message408 = 0x4FC;
        failed = func_800957C0(menu, selected, 1, 0, 0) != 1;
        if (failed) {
            func_801E9F90_13BEB0(16, 0);
        } else {
            OvlPair pair;
            func_801E9F90_13BEB0(16, 1);
            pair = func_80097F20(&D_801404E0);
            OvlItem *selected_item = pair.item;
            D_801F51E8_147108->item118 = selected_item;
        }
        break;
    }
    case 0x74:
        if (D_801F51E8_147108->item118) {
            func_80083C90(text, func_800AE674(D_801F51E8_147108->item118));
            func_801EB348_13D268(text);
        }
        break;
    case 0x75:
        if (D_801F51E8_147108->item118) {
            OvlCollection *collection = player_collection(D_801476B8);
            if (collection) {
                func_800CD364(collection, D_801F51E8_147108->item118);
                D_801F51E8_147108->item118 = 0;
            }
        }
        break;
    case 0x76: func_801E9F90_13BEB0(16, func_800D1054(D_801F51E8_147108->args[1].byte[3])); break;
    case 0x77: {
        OvlSaveRecord *save = func_800C9E10();
        if (save) {
            if (D_801F51E8_147108->args[1].word == 0) {
                func_800265E0(auxiliary, sizeof(auxiliary));
                func_800CAD04(save, auxiliary, text);
                func_801EB348_13D268(text);
            } else {
                /* ORIGINAL BUG:801F1300 branches here, bypassing memset
                 * at801F1308 and CAD04 at801F131C. ROM801F1334/801F1338
                 * passes unset sp+18 to801EB348 ->801ECF5C. The named
                 * case0x77 exception is ORIGINAL-BUG-UNINIT.md; see the
                 * declaration for measured defined-value alternatives. */
                func_801EB348_13D268(auxiliary);
            }
        }
        break;
    }
    case 0x78: {
        s32 i;
        OvlCollection *collection;
        func_801E9F90_13BEB0(16, 0); func_801E9F90_13BEB0(17, 0);
        collection = player_collection(D_801476B8);
        i = collection->table04->count24((u8 *)collection + collection->table04->delta20);
        while (--i != -1) {
            OvlItem *item = collection->table04->item3C((u8 *)collection + collection->table04->delta38, i);
            if (item->field01 != 0xAC) {
                func_801E9F90_13BEB0(17, 1);
                return;
            }
        }
        if ((u32)collection->table04->count24((u8 *)collection + collection->table04->delta20) < 11) func_801E9F90_13BEB0(16, 1);
        break;
    }
    case 0x79: func_801E9F90_13BEB0(23, func_801E9F70_13BE90(D_801F51E8_147108->args[1].word)); break;
    case 0x7A: func_801E9F90_13BEB0(D_801F51E8_147108->args[1].word, func_801E9F70_13BE90(23)); break;
    case 0x7C: D_801F51E8_147108->field9FC = 1; break;
    case 0x7E:
        func_8006D418(); func_8006D438(); func_801E8F20_13AE40(1);
        break;
    case 0x7F: {
        OvlSaveRecord *save = func_800C9E10();
        if (save) func_800CABD8(save);
        break;
    }
    case 0x80: {
        void *item = func_800EB9FC(D_801476B8);
        if (item) func_800AE518(item, D_801476B8, 0, 0);
        break;
    }
    case 0x81: func_801EEA38_140958(); break;
    case 0x82: func_801E9F90_13BEB0(16, func_800AC1FC()); break;
    case 0x83: func_801EEAEC_140A0C(); break;
    }
}
}