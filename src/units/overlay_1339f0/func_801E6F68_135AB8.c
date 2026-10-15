#include "common.h"
#include "vi_mode.h"

/* libultra field views from the reviewed A-menu source; widths and offsets unchanged. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* libultra controller / controller-pak records (os_cont.h, os_pfs.h layouts; the resident
 * definitions func_80027910, func_800277B8, func_8002E930 and func_8002E470 use the same). */
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
typedef struct {
    u16 type;
    u8 status;  /* +0x2: lbu 801E6FE8 (CONT_CARD_ON, bit 0) */
    u8 errno;   /* +0x3: lbu 801E6FDC */
} OSContStatus;
typedef struct {
    u16 button; /* +0x0: lhu 801E7054/7184/7714/7724/7760 */
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;
typedef struct {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32];
    u8 label[32];
    s32 version;
    s32 dir_size;
    s32 inode_table;
    s32 minode_table;
    s32 dir_table;
    s32 inode_start_page;
    u8 banks;
    u8 activebank;
} OSPfs;
typedef struct {
    u32 file_size;      /* +0x00: lw 801E7590 */
    u32 game_code;      /* +0x04: lw 801E753C */
    u16 company_code;   /* +0x08: lhu 801E7530 */
    char ext_name[4];   /* +0x0A: address 801E7534 */
    char game_name[16]; /* +0x0E: address 801E7540 */
} OSPfsState;


extern signed long func_80027910(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *status); /* osContInit */
extern s32 func_80027730(OSMesgQueue *mq);                                     /* osContStartReadData */
extern void func_800277B8(OSContPad *pad);                                     /* osContGetReadData */
extern signed long func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, signed long flags); /* osRecvMesg */
extern s32 func_8002E930(OSMesgQueue *mq, OSPfs *pfs, s32 channel);            /* osPfsInitPak */
extern s32 func_8002F420(OSPfs *pfs);                                          /* osPfsRepairId */
extern s32 func_8002EEE0(OSPfs *pfs, s32 *max_files, s32 *files_used);         /* osPfsNumFiles */
extern s32 func_8002E470(OSPfs *pfs, s32 file_no, OSPfsState *state);          /* osPfsFileState */
extern s32 func_8002E610(OSPfs *pfs, s32 *bytes_not_used);                     /* osPfsFreeBlocks */
extern s32 func_8002E250(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name); /* osPfsDeleteFile */
extern void func_800343C0(void *framebuffer);                                  /* osViSwapBuffer */
extern void func_80033CC0(u8 active);                                          /* osViBlack */
extern void func_800347A0(void);

extern void func_801E67D0_135320(void *buffer);
extern void func_801E6A70_1355C0(s32 c, u16 color, u16 shade);
extern void func_801E6BC4_135714(s32 x, s32 y);
extern void func_801E6C30_135780(char *text, u16 color, u16 shade);
extern void func_801E6C80_1357D0(s32 column, s32 row, u16 a, u16 b, u16 c);
extern void func_801E6EC4_135A14(u16 pages);

/* Overlay A only: original pointer words at ROM [0x1369B0,0x1369B8).
 * One joint owner for the menu and adjacent allocator. Other consumers remain
 * extern. No following data or BSS is claimed by this definition. */
u16 *D_801E7E60_1369B0 = (u16 *)0x80300000;
void *D_801E7E64_1369B4 = (void *)0x80200000;
extern u16 D_801E7E84_1369D4[];
extern char D_801E81E8_136D38[], D_801E81F8_136D48[], D_801E8208_136D58[], D_801E8218_136D68[], D_801E8228_136D78[], D_801E8230_136D80[], D_801E8248_136D98[], D_801E8250_136DA0[];
extern OSPfsState D_801E84C0_137010[16];
/* ORIGINAL BUG: this is exactly s32[16], RAM [0x801E86C0,0x801E8700).
 * The original searches read [-1] at [0x801E86BC,0x801E86C0) and [16] at
 * [0x801E8700,0x801E8704). The former overlaps the last two game_name bytes
 * and implicit tail alignment of D_801E84C0_137010[15], not a status element.
 * The latter has no A object: it is shared-overlay residue below the heap
 * starting at 0x801F7200 (func_8006A840), not an added seventeenth element.
 * A B-image load puts E7AE0014 there (ROM 0x13A620); an A load ends at
 * 0x801E8270 and leaves that word untouched. Without a preceding B load,
 * startup does not establish its value. Neither read changes the final
 * cursor for any word value, but zero/nonzero selects a different exit path.
 * Evidence: A bss.bss.s reserves 0x200/0x40 bytes for the two arrays;
 * ROM 0x135FE0/0x13601C loads before bounds checks. See
 * scratch/omp/wave29/ovl-c3/obug/results.json, scratch/omp/wave29/ovl-c3/mechanism.json
 * and scratch/omp/rulings/ORIGINAL-BUG-OVERREAD.md.
 * Extents stay unchanged; the ruling does not authorize return-label tricks. */
extern s32 D_801E86C0_137210[16];

/* Controller-pak file manager: samples START on the first connected controller,
 * initialises (or repairs) the pak, lists its 16 notes and permits deletion.
 * Returns 0 on setup failure, 2 without START, or 1 / 3 on leaving the menu.
 * Every failure is an ordinary early return; no label or goto reaches the
 * zero-return block. The original-bug exception covers only the two reads below.
 * The scan adopts a controller as the channel only once its pak is known to be
 * present. That order is why the no-pak `return 0` is the ROM's zero block at
 * 0x801E7770: loop.c moves that loop-exit block (an if-guarded exit ending in an
 * unconditional jump, followed by `channel = i`) to the barrier before the return
 * label, and jump2 cross-jumps the other two setup returns onto it. */
s32 func_801E6F68_135AB8(OSMesgQueue *mq, OSMesgQueue *retraceQueue) {
    OSContStatus status[4];
    OSContPad pad[4];
    OSPfs pfs;
    u8 bitpattern;
    OSMesg msg;
    s32 maxFiles;
    s32 filesUsed;
    s32 pages;
    OSContPad *cont;
    s32 ok;
    s32 channel;
    s32 i;
    s32 cursor, prevCursor;
    s32 state, prevState; /* state also carries the osPfsInitPak result before the menu */
    s32 blink;
    u16 counter;
    u16 trigger, repeat;
    u16 color;

    func_80027910(mq, &bitpattern, status);
    func_80027730(mq);
    func_8002FEA0(mq, &msg, 1);
    func_800277B8(pad);
    channel = 0;
    if (bitpattern == 0) return 0;
    for (i = 0; i < 4; i++) {
        if (status[i].errno == 0) {
            if (!(status[i].status & 1)) return 0; /* no pak in the controller */
            channel = i;                           /* first controller with a pak */
            break;
        }
    }
    if (i == 4) return 0;
    cont = &pad[(u8)channel];
    func_80027730(mq);
    func_8002FEA0(mq, &msg, 1);
    func_800277B8(pad);
    if (cont->button & 0x1000) {
    state = func_8002E930(mq, &pfs, (u8)channel);
    if (state != 0 && state != 10) return 0;
    func_801E67D0_135320(D_801E7E64_1369B4);
    for (i = 0; i < 640 * 480 * sizeof(u16) / sizeof(u32); i++) ((u32 *)D_801E7E60_1369B0)[i] = 0;
    func_800343C0(D_801E7E60_1369B0);
    func_800341B0(&D_800374E0[9]);
    func_80033CC0(0);
    if (state == 10) {
        func_801E6BC4_135714(0xC0, 0x8C);
        func_801E6C30_135780(D_801E81E8_136D38, 0xF801, 0x52BF);
        func_801E6BC4_135714(0xE0, 0x104);
        func_801E6C30_135780(D_801E81F8_136D48, 0x52BF, 0x52BF);
        func_801E6BC4_135714(0xC0, 0x118);
        func_801E6C30_135780(D_801E8208_136D58, 0x52BF, 0x52BF);
        func_800347A0();
        func_8002FEA0(retraceQueue, &msg, 1);
        do {
            func_80027730(mq);
            func_8002FEA0(mq, &msg, 1);
            func_800277B8(pad);
            if (cont->button != 0) {
                if (cont->button == 0x30) func_8002F420(&pfs);
                state = func_8002E930(mq, &pfs, (u8)channel);
            }
        } while (state != 0);
    }
    } else {
        return 2;
    }
    if (func_8002EEE0(&pfs, &maxFiles, &filesUsed) != 0) return 0;
    for (i = 0; i < 16; i++) D_801E86C0_137210[i] = func_8002E470(&pfs, i, &D_801E84C0_137010[i]);
    if (func_8002E610(&pfs, &pages) != 0) return 0;
    ok = 1;
    pages >>= 8;
redraw:
    for (i = 0; i < 640 * 480 * sizeof(u16) / sizeof(u32); i++) ((u32 *)D_801E7E60_1369B0)[i] = 0;
    switch (ok) {
    case 1:
        func_801E6BC4_135714(0x60, 0x24);
        func_801E6C30_135780(D_801E8218_136D68, 0x52BF, 1);
        func_801E6BC4_135714(0x190, 0x24);
        func_801E6C30_135780(D_801E8228_136D78, 0x52BF, 1);
        func_801E6A70_1355C0(0x12, 0x52BF, 1);
        func_801E6A70_1355C0(0x3C, 0x52BF, 1);
        func_801E6A70_1355C0(0x10, 0x52BF, 1);
        func_801E6BC4_135714(0x60, 0x40);
        func_801E6C30_135780(D_801E8230_136D80, 0xFFFF, 1);
        func_801E6BC4_135714(0x156, 0x198);
        func_801E6C30_135780(D_801E8248_136D98, 0xFFFF, 1);
        func_801E6EC4_135A14(pages);
        for (i = 0; i < 17; i++) func_801E6C80_1357D0(0, i, 0xFFFF, 1, 1);
        break;
    case 0:
        func_801E6BC4_135714(0xA0, 0xC8);
        func_801E6C30_135780(D_801E8250_136DA0, 0x52BF, 0x52BF);
        filesUsed = 0;
        break;
    }
    func_800347A0();
    counter = 0;
    func_8002FEA0(retraceQueue, &msg, 1);
    repeat = 0;
    cursor = 16;
    trigger = 0x400;
    blink = 0;
    state = 0;
    for (;;) {
        prevCursor = cursor;
        prevState = state;
        switch (state) {
        case 0:
            if (trigger & 0x8000) {
                if (cursor == 16) return ok ? 3 : 1;
                for (i = 0; i < 17; i++) func_801E6C80_1357D0(0, i, 0x843, 1, 1);
                state = 2;
            } else if ((trigger | repeat) & 0x800) {
                cursor--;
                /* ORIGINAL BUG: at cursor == -1, read 0x801E86BC..86BF:
                 * record 15's game_name[14..15] plus two implicit alignment
                 * bytes. ROM 0x135FE0 (RAM 0x801E7490) loads before bltz.
                 * The raw word is used only by bnez at 0x801E7494, then
                 * overwritten by -1 in its delay slot. Zero exits directly;
                 * nonzero takes bltz first. Both wrap cursor to 16, with no
                 * boundary store or propagated value. Pak data or B residue
                 * (C7AC0010 at ROM 0x13A5DC before A record writes) can change
                 * that branch path, not the resulting menu state. */
                while (D_801E86C0_137210[cursor] != 0 && cursor >= 0) cursor--;
                if (cursor == -1) cursor = 16;
            } else if ((trigger | repeat) & 0x400) {
            down:
                if (cursor == 16) cursor = -1;
                cursor++;
                /* ORIGINAL BUG: at cursor == 16, read 0x801E8700..8703,
                 * past A BSS, in the shared overlay slot rather than heap.
                 * ROM 0x13601C (RAM 0x801E74CC) loads before slti cursor,16.
                 * If B preceded A and its bytes remain, the word is E7AE0014
                 * from func_801E8634_13A554 (B ROM 0x13A620); otherwise its
                 * value is not established. beqz at 0x801E74D0 uses the word,
                 * whose delay-slot slti overwrites it with zero. A zero word
                 * exits directly; any nonzero word executes the bound-exit
                 * branch at 0x801E74D8. Both leave cursor == 16 and identical
                 * menu state. The value is not masked or stored; only the
                 * executed exit path (two extra instructions) depends on it. */
                while (D_801E86C0_137210[cursor] != 0 && cursor < 16) cursor++;
            } else if (trigger & 0x4000) {
                cursor = 16;
            }
            break;
        case 1: {
            OSPfsState *file = &D_801E84C0_137010[cursor];
            if (trigger & 0x400) state = 2;
            if (trigger & 0x4000) state = 0;
            if (trigger & 0x8000) {
                if (func_8002E250(&pfs, file->company_code, file->game_code, (u8 *)file->game_name,
                                  (u8 *)file->ext_name) != 0) {
                    ok = 0;
                    goto redraw;
                }
                func_801E6C80_1357D0(1, cursor, 1, 1, prevState);
                func_801E6C80_1357D0(2, cursor, 1, 1, prevState);
                D_801E86C0_137210[cursor] = -1;
                pages += (u8)((file->file_size + 0xFF) >> 8);
                func_801E6EC4_135A14(pages);
                state = 0;
                goto down;
            }
            break;
        }
        case 2:
            if (trigger & 0x800) state = 1;
            if (trigger & 0xC000) state = 0;
            break;
        }
        if (filesUsed == 0) cursor = 16;
        if (state != prevState) {
            u16 shade = 0xFFFF;
            if (state == 0) {
                for (i = 0; i < 17; i++) func_801E6C80_1357D0(0, i, 0xFFFF, 1, 1);
                shade = 1;
            }
            func_801E6C80_1357D0(1, prevCursor, 0xFFFF, shade, 1);
            func_801E6C80_1357D0(2, prevCursor, 0xFFFF, shade, 1);
            blink = 0;
        } else if (cursor != prevCursor) {
            func_801E6C80_1357D0(0, prevCursor, 0xFFFF, 1, 1);
            blink = 0;
        }
        color = D_801E7E84_1369D4[blink / 3];
        func_801E6C80_1357D0(state, cursor, color, color, color);
        if (++blink == 30) blink = 0;
        func_80027730(mq);
        func_800347A0();
        func_8002FEA0(retraceQueue, &msg, 1);
        if ((s32)msg == 1) for (;;) ;
        func_8002FEA0(mq, &msg, 1);
        trigger = cont->button;
        func_800277B8(pad);
        repeat = cont->button & trigger;
        if (repeat) {
            counter++;
            if ((counter >> 5) == 0 || (counter & 7)) repeat = 0;
        } else {
            counter = 0;
        }
        trigger = cont->button & (cont->button ^ trigger);
    }
}

extern char D_801E825C_136DAC[];
extern char D_801E8264_136DB4[];
extern void *func_8006A8D8(char *name, u32 size);
extern void func_8006A9D4(void *address);

/* Original allocator behavior: test only workspace allocation, release only
 * the framebuffer, and return zero. The menu's preceding s32 definition is
 * the call contract; no separate void declaration or second data owner. */
s32 func_801E77A4_1362F4(void *context, void *queue) {
    D_801E7E60_1369B0 = func_8006A8D8(D_801E825C_136DAC, 0x96000);
    D_801E7E64_1369B4 = func_8006A8D8(D_801E8264_136DB4, 0x1C00);
    if (D_801E7E64_1369B4 != 0) {
        func_801E6F68_135AB8(context, queue);
    }
    func_8006A9D4(D_801E7E60_1369B0);
    return 0;
}
