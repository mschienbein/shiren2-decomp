/*
 * Reconstructed from Japanese rev0 ROM 0x1400..0x1494. The original names of
 * this routine and its static byte are unknown. SDK semantics were checked
 * against ultralib e24c836796df4bf520ff8b11a5c9d2cea3a66cbd; this translation
 * unit is independently written from the instructions and data references.
 */
#if !defined(SDK_K_ROM) && !defined(SDK_L_ROM)
#error Select the recorded K/L active-body hypothesis from the TU profile
#endif

typedef signed long s32;
typedef unsigned long u32;
typedef unsigned char u8;

typedef char target_long_is_32[(sizeof(u32) == 4) ? 1 : -1];
typedef char target_pointer_is_32[(sizeof(void *) == 4) ? 1 : -1];
typedef char target_byte_is_8[(sizeof(u8) == 1) ? 1 : -1];

/* Cross-TU contracts: both definitions use common.h's int-based s32/u32. */
extern signed int func_80025EA0(void);
extern unsigned int func_800340F0(void *address);

s32 func_80026000(void *buffer, u32 length)
{
    static u8 buffer_adjustment_pending = 0;
    u8 *dma_buffer;

    if (func_80025EA0()) {
        return -1;
    }

    dma_buffer = buffer;
    if (buffer_adjustment_pending) {
        /* The original hardware workaround subtracts from a 32-bit address. */
        dma_buffer = (u8 *)((u32)buffer - 0x2000UL);
    }

    if ((((u32)buffer + length) & 0x1FFF) == 0) {
        buffer_adjustment_pending = 1;
    } else {
        buffer_adjustment_pending = 0;
    }

    *(volatile u32 *)0xA4500000UL = func_800340F0(dma_buffer);
    *(volatile u32 *)0xA4500004UL = length;
    return 0;
}
