#include "pi_handle_view.h"

/* Function-scope cache lifetime hypothesis; effects retain original order. */
PiResult func_80029C70(PiHandleView *handle, PiWord device_address, PiWord *output)
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
    *output = PI_IO_READ(handle->base_address | device_address);
    return 0;
}
