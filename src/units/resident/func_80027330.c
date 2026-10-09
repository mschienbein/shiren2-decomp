#include "controller_ram.h"

s32 func_80027330(void *queue, s32 channel, u16 block, u8 *destination)
{
    s32 status = 0;
    s32 i;
    u8 *cursor;
    u8 *data;
    s32 retry = 2;

    func_800322C4();

    do {
        cursor = (u8 *)&D_80041350;
        if (D_80039018 != 2 || D_80036F70 != channel) {
            D_80039018 = 2;
            D_80036F70 = channel;
            for (i = 0; i < channel; i++) {
                *cursor++ = 0;
            }
            D_80041350.pifstatus = 1;
            ((ResidentControllerRamPacket *)cursor)->dummy = 0xFF;
            ((ResidentControllerRamPacket *)cursor)->transmit_size = 3;
            ((ResidentControllerRamPacket *)cursor)->receive_size = 0x21;
            ((ResidentControllerRamPacket *)cursor)->command = 2;
            ((ResidentControllerRamPacket *)cursor)->checksum = 0xFF;
            ((ResidentControllerRamPacket *)cursor)->end = 0xFE;
        } else {
            cursor += channel;
        }
        ((ResidentControllerRamPacket *)cursor)->address_high = block >> 3;
        ((ResidentControllerRamPacket *)cursor)->address_low = (block << 5) | func_80027DB0(block);

        status = func_80032500(1, &D_80041350);
        func_8002FEA0(queue, 0, 1);
        status = func_80032500(0, &D_80041350);
        func_8002FEA0(queue, 0, 1);

        status = (((ResidentControllerRamPacket *)cursor)->receive_size & 0xC0) >> 4;
        if (status == 0) {
            data = ((ResidentControllerRamPacket *)cursor)->data;
            if (func_80027E1C(data) != ((ResidentControllerRamPacket *)cursor)->checksum) {
                status = func_8002E720(queue, channel);
                if (status != 0) {
                    break;
                } else {
                    status = 4;
                }
            } else {
                func_800262C0(data, destination, 32);
            }
        } else {
            status = 1;
        }
    } while (status == 4 && retry-- >= 0);

    func_80032330();
    return status;
}
