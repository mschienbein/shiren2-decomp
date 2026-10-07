#include "controller_crc.h"

u8 func_80027DB0(u32 address)
{
    u32 crc = 0;
    u32 bit = 0x400;

    address &= 0xFFFF;
    do {
        crc <<= 1;
        if (address & bit) {
            if (crc & 0x20) {
                crc ^= 0x14;
            } else {
                crc += 1;
            }
        } else if (crc & 0x20) {
            crc ^= 0x15;
        }
        bit >>= 1;
    } while (bit != 0);

    for (bit = 5; bit != 0; bit--) {
        crc <<= 1;
        if (crc & 0x20) {
            crc ^= 0x15;
        }
    }
    return crc & 0x1F;
}

u8 func_80027E1C(const u8 *data)
{
    u32 crc = 0;
    u32 remaining = 32;
    u32 bit;
    u8 byte;

    do {
        bit = 0x80;
        byte = *data;
        do {
            crc <<= 1;
            if (byte & bit) {
                if (crc & 0x100) {
                    crc ^= 0x84;
                } else {
                    crc += 1;
                }
            } else if (crc & 0x100) {
                crc ^= 0x85;
            }
            bit >>= 1;
        } while (bit != 0);
        remaining--;
        data++;
    } while (remaining != 0);

    /* The byte loop leaves remaining at zero; process eight zero input bits. */
    do {
        crc <<= 1;
        if (crc & 0x100) {
            crc ^= 0x85;
        }
        remaining++;
    } while (remaining < 8);
    return crc & 0xFF;
}
