#include "pi_handle_view.h"

/* Function-scope cache lifetime hypothesis; effects retain original order. */
PiResult func_80029A80(PiHandleView *handle, PiResult direction,
                     PiWord device_address, void *dram_address, PiWord size)
{
    PiWord stat;
    PiWord domain;
    PiHandleView *cached;

    stat = PI_IO_READ(PI_STATUS);
    while (stat & 3UL)
        stat = PI_IO_READ(PI_STATUS);
    domain = handle->domain;
    if (D_800372A0[domain]->type != handle->type) {
        cached = D_800372A0[domain];
        if (domain == 0UL) {
            PI_UPDATE_TIMING(handle, PI_DOMAIN1_LATENCY, latency);
            PI_UPDATE_TIMING(handle, PI_DOMAIN1_PAGE_SIZE, page_size);
            PI_UPDATE_TIMING(handle, PI_DOMAIN1_RELEASE, release_duration);
            PI_UPDATE_TIMING(handle, PI_DOMAIN1_PULSE, pulse);
        } else {
            PI_UPDATE_TIMING(handle, PI_DOMAIN2_LATENCY, latency);
            PI_UPDATE_TIMING(handle, PI_DOMAIN2_PAGE_SIZE, page_size);
            PI_UPDATE_TIMING(handle, PI_DOMAIN2_RELEASE, release_duration);
            PI_UPDATE_TIMING(handle, PI_DOMAIN2_PULSE, pulse);
        }
        cached->type = handle->type;
        cached->latency = handle->latency;
        cached->page_size = handle->page_size;
        cached->release_duration = handle->release_duration;
        cached->pulse = handle->pulse;
    }
    PI_IO_WRITE(PI_DRAM_ADDRESS, func_800340F0(dram_address));
    PI_IO_WRITE(PI_CART_ADDRESS, (handle->base_address | device_address) & PI_PHYSICAL_MASK);

    switch (direction) {
        case 0:
            PI_IO_WRITE(PI_WRITE_LENGTH, size - 1UL);
            break;
        case 1:
            PI_IO_WRITE(PI_READ_LENGTH, size - 1UL);
            break;
        default:
            return -1;
    }
    return 0;
}
