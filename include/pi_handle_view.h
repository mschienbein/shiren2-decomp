#ifndef SHIREN2_P30_PI_HANDLE_VIEW_H
#define SHIREN2_P30_PI_HANDLE_VIEW_H

/* Source-only, target-qualified views. No storage or historical API is defined. */
typedef unsigned char PiByte;
typedef unsigned long PiWord;
typedef signed long PiResult;

typedef char pi_word_is_32[(sizeof(PiWord) == 4) ? 1 : -1];
typedef char pi_pointer_is_32[(sizeof(void *) == 4) ? 1 : -1];

/* Only offsets 04..09 and 0C are accessed by this family. The owner may be larger. */
typedef struct PiHandleView {
    PiByte opaque00[4];
    PiByte type;
    PiByte latency;
    PiByte page_size;
    PiByte release_duration;
    PiByte pulse;
    PiByte domain;
    PiByte opaque0A[2];
    PiWord base_address;
} PiHandleView;

/* Constants follow the actual original accesses, not a symbolic-lowering remedy. */
#define PI_DRAM_ADDRESS      0xA4600000UL
#define PI_CART_ADDRESS      0xA4600004UL
#define PI_READ_LENGTH       0xA4600008UL
#define PI_WRITE_LENGTH      0xA460000CUL
#define PI_STATUS            0xA4600010UL
#define PI_DOMAIN1_LATENCY   0xA4600014UL
#define PI_DOMAIN1_PULSE     0xA4600018UL
#define PI_DOMAIN1_PAGE_SIZE 0xA460001CUL
#define PI_DOMAIN1_RELEASE   0xA4600020UL
#define PI_DOMAIN2_LATENCY   0xA4600024UL
#define PI_DOMAIN2_PULSE     0xA4600028UL
#define PI_DOMAIN2_PAGE_SIZE 0xA460002CUL
#define PI_DOMAIN2_RELEASE   0xA4600030UL
#define PI_UNCACHED_SEGMENT   0xA0000000UL
#define PI_PHYSICAL_MASK     0x1FFFFFFFUL

/* Target MMIO mapping: volatile 32-bit access, with the original KSEG1 OR. */
#define PI_IO_WORD(address) \
    (*(volatile PiWord *)((address) | PI_UNCACHED_SEGMENT))
#define PI_IO_READ(address) PI_IO_WORD(address)
#define PI_IO_WRITE(address, value) (PI_IO_WORD(address) = (value))

/* Incomplete table: two original pointer words are observed; no new owner size. */
extern PiHandleView *D_800372A0[];

/* The resident definition returns common.h's unsigned-int u32; PI callers
 * convert the same 32-bit physical address to PiWord for the MMIO store. */
extern unsigned int func_800340F0(void *address);

extern PiResult func_80029A80(PiHandleView *handle, PiResult direction,
                            PiWord device_address, void *dram_address, PiWord size);
extern PiResult func_80029C70(PiHandleView *handle, PiWord device_address,
                            PiWord *output);
extern PiResult func_80029DE0(PiHandleView *handle, PiWord device_address,
                            PiWord value);

#define PI_UPDATE_TIMING(handle, address, field) \
    if (cached->field != (handle)->field) \
        PI_IO_WRITE(address, (handle)->field)

/* Inline source macro, not a new runtime helper. Statements retain original order. */
#define PI_SYNCHRONIZE(handle, stat, domain) \
    do { \
        (stat) = PI_IO_READ(PI_STATUS); \
        while ((stat) & 3UL) \
            (stat) = PI_IO_READ(PI_STATUS); \
        (domain) = (handle)->domain; \
        if (D_800372A0[(domain)]->type != (handle)->type) { \
            PiHandleView *cached = D_800372A0[(domain)]; \
            if ((domain) == 0UL) { \
                PI_UPDATE_TIMING(handle, PI_DOMAIN1_LATENCY, latency); \
                PI_UPDATE_TIMING(handle, PI_DOMAIN1_PAGE_SIZE, page_size); \
                PI_UPDATE_TIMING(handle, PI_DOMAIN1_RELEASE, release_duration); \
                PI_UPDATE_TIMING(handle, PI_DOMAIN1_PULSE, pulse); \
            } else { \
                PI_UPDATE_TIMING(handle, PI_DOMAIN2_LATENCY, latency); \
                PI_UPDATE_TIMING(handle, PI_DOMAIN2_PAGE_SIZE, page_size); \
                PI_UPDATE_TIMING(handle, PI_DOMAIN2_RELEASE, release_duration); \
                PI_UPDATE_TIMING(handle, PI_DOMAIN2_PULSE, pulse); \
            } \
            cached->type = (handle)->type; \
            cached->latency = (handle)->latency; \
            cached->page_size = (handle)->page_size; \
            cached->release_duration = (handle)->release_duration; \
            cached->pulse = (handle)->pulse; \
        } \
    } while (0)

#endif
