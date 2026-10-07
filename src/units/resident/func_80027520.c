#include "controller_ram.h"

s32 func_80027520(void *queue, s32 channel, u16 block,
                  const u8 *data, s32 force)
{
    s32 status;
    s32 retry = 2;
    s32 index;
    u8 checksum;
    u8 *cursor;

    if (force != 1 && block < 7) {
        if (block != 0) {
            return 0;
        }
    }

    func_800322C4();
    do {
        cursor = D_80041350;
        if (D_80039018 != 3 || D_80036F70 != channel) {
            D_80039018 = 3;
            D_80036F70 = channel;
            for (index = 0; index < channel; index++) {
                *cursor++ = 0;
            }
            D_8004138C = 1;
            ((ResidentControllerRamPacket *)cursor)->dummy = 0xff;
            ((ResidentControllerRamPacket *)cursor)->transmit_size = 0x23;
            ((ResidentControllerRamPacket *)cursor)->receive_size = 1;
            ((ResidentControllerRamPacket *)cursor)->command = 3;
            ((ResidentControllerRamPacket *)cursor)->checksum = 0xff;
            ((ResidentControllerRamPacket *)cursor)->end = 0xfe;
        } else {
            cursor += channel;
        }

        ((ResidentControllerRamPacket *)cursor)->address_high = (u8)(block >> 3);
        ((ResidentControllerRamPacket *)cursor)->address_low = (u8)((block << 5) | func_80027DB0(block));
        func_800262C0(data, ((ResidentControllerRamPacket *)cursor)->data, 32);
        func_80032500(1, D_80041350);
        checksum = func_80027E1C(data);
        func_8002FEA0(queue, (void **)0, 1);
        func_80032500(0, D_80041350);
        func_8002FEA0(queue, (void **)0, 1);

        status = (((ResidentControllerRamPacket *)cursor)->receive_size & 0xc0) >> 4;
        if (status == 0) {
            if (checksum != ((ResidentControllerRamPacket *)cursor)->checksum) {
                status = func_8002E720(queue, channel);
                if (status != 0) {
                    break;
                }
                status = 4;
            }
        } else {
            status = 1;
        }
    } while (status == 4 && retry-- >= 0);

    func_80032330();
    return status;
}
