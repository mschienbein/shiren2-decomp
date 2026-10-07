#include "contpfs_types.h"

/* Original cache data; all padding is generated naturally. */

s32 D_80036F60 = -1;

u8 D_80036F64 = 250;

ResidentInode D_800411D0;

/* Adapted leaf 80026800; see source-provenance.json. */

u16 func_80026800(u8 *bytes, s32 length)
{
    s32 index;
    u32 total = 0;
    u8 *cursor = bytes;

    for (index = 0; index < length; index++) {
        total += *cursor++;
    }

    return (u16)total;
}

/* Adapted leaf 80026834; see source-provenance.json. */

s32 func_80026834(u16 *record, u16 *sum, u16 *inverse_sum)
{
    u16 value = 0;
    u32 offset;

    *inverse_sum = 0;
    *sum = 0;
    for (offset = 0; offset < 28; offset += 2) {
        value = *(u16 *)((u8 *)record + offset);
        *sum += value;
        *inverse_sum += ~value;
    }
    return 0;
}

/* Adapted leaf 80026878; see source-provenance.json. */

s32 func_80026878(ResidentPfs *pfs, ResidentPackId *old_id, ResidentPackId *new_id)
{
    u8 modified[32];
    u8 observed[32];
    u16 blocks[4];
    s32 status;
    s32 byte_index;
    s32 bank;
    u8 mask;

    new_id->field00 = (u32)-1;
    new_id->field04 = func_8002A9B0();
    new_id->field08 = old_id->field08;
    new_id->field10 = old_id->field10;
    bank = 0;

    if (pfs->field65 != 0) {
        status = func_8002F640(pfs, 0);
        if (status != 0) {
            return status;
        }
    }

    do {
        status = func_8002F640(pfs, (u8)bank);
        if (status != 0) {
            return status;
        }
        status = func_80027330(pfs->field04, pfs->field08, 0, modified);
        if (status != 0) {
            return status;
        }

        modified[0] = (u8)(bank | 0x80);
        for (byte_index = 1; byte_index < 32; byte_index++) {
            modified[byte_index] = (u8)~modified[byte_index];
        }

        status = func_80027520(pfs->field04, pfs->field08, 0, modified, 0);
        if (status != 0) {
            return status;
        }
        status = func_80027330(pfs->field04, pfs->field08, 0, observed);
        if (status != 0) {
            return status;
        }

        for (byte_index = 0; byte_index < 32; byte_index++) {
            if (observed[byte_index] != modified[byte_index]) {
                break;
            }
        }
        if (byte_index != 32) {
            break;
        }

        if (bank > 0) {
            status = func_8002F640(pfs, 0);
            if (status != 0) {
                return status;
            }
            status = func_80027330(pfs->field04, pfs->field08, 0, modified);
            if (status != 0) {
                return status;
            }
            if (modified[0] != 0x80) {
                break;
            }
        }

        bank++;
    } while (bank < 62);

    if (pfs->field65 != 0) {
        status = func_8002F640(pfs, 0);
        if (status != 0) {
            return status;
        }
    }

    mask = (bank > 0) ? 1 : 0;
    new_id->field18 = (old_id->field18 & (u16)~1) | mask;
    new_id->field1a = (u8)bank;
    new_id->field1b = old_id->field1b;

    /* This checksum loop is inlined in the original 80026878 body. */
    func_80026834((u16 *)new_id, &new_id->field1c, &new_id->field1e);

    blocks[0] = 1;
    blocks[1] = 3;
    blocks[2] = 4;
    blocks[3] = 6;
    for (byte_index = 0; byte_index < 4; byte_index++) {
        status = func_80027520(pfs->field04, pfs->field08,
                              blocks[byte_index], (u8 *)new_id, 1);
        if (status != 0) {
            return status;
        }
    }

    status = func_80027330(pfs->field04, pfs->field08, 1, modified);
    if (status != 0) {
        return status;
    }
    for (byte_index = 0; byte_index < 32; byte_index++) {
        if (modified[byte_index] != ((u8 *)new_id)[byte_index]) {
            return 11;
        }
    }
    return 0;
}

/* Adapted leaf 80026B64; see source-provenance.json. */

s32 func_80026B64(ResidentPfs *pfs, ResidentPackId *record)
{
    u16 blocks[4];
    s32 result;
    u16 sum;
    u16 inverse_sum;
    s32 valid;
    s32 index;

    if (pfs->field65 != 0) {
        result = func_8002F640(pfs, 0);
        if (result != 0) {
            return result;
        }
    }

    blocks[0] = 1;
    blocks[1] = 3;
    blocks[2] = 4;
    blocks[3] = 6;
    for (valid = 1; valid < 4; valid++) {
        result = func_80027330(pfs->field04, pfs->field08, blocks[valid], (u8 *)record);
        if (result != 0) {
            return result;
        }
        func_80026834((u16 *)record, &sum, &inverse_sum);
        if (record->field1c == sum && record->field1e == inverse_sum) {
            break;
        }
    }
    if (valid == 4) {
        return 10;
    }

    for (index = 0; index < 4; index++) {
        if (index != valid) {
            result = func_80027520(pfs->field04, pfs->field08, blocks[index], (u8 *)record, 1);
            if (result != 0) {
                return result;
            }
        }
    }
    return 0;
}

/* Adapted leaf 80026CC8; see source-provenance.json. */

s32 func_80026CC8(ResidentPfs *pfs)
{
    u16 sum;
    u16 inverse_sum;
    u8 buffer[32];
    ResidentPackId replacement;
    s32 result;
    ResidentPackId *record;

    if (pfs->field65 != 0) {
        result = func_8002F640(pfs, 0);
        if (result != 0) {
            return result;
        }
    }
    result = func_80027330(pfs->field04, pfs->field08, 1, buffer);
    if (result != 0) {
        return result;
    }
    func_80026834((u16 *)buffer, &sum, &inverse_sum);
    record = (ResidentPackId *)buffer;
    if (record->field1c != sum || record->field1e != inverse_sum) {
        result = func_80026B64(pfs, record);
        if (result == 10) {
            result = func_80026878(pfs, record, &replacement);
            if (result != 0) {
                return result;
            }
            record = &replacement;
        } else if (result != 0) {
            return result;
        }
    }

    if ((record->field18 & 1) == 0) {
        result = func_80026878(pfs, record, &replacement);
        if (result != 0) {
            return result;
        }
        record = &replacement;
        if ((record->field18 & 1) == 0) {
            return 11;
        }
    }

    func_800262C0(record, pfs->field0c, 32);
    pfs->field4c = record->field1b;
    pfs->field64 = record->field1a;
    pfs->field60 = pfs->field64 * 2 + 3;
    pfs->field50 = 16;
    pfs->field54 = 8;
    pfs->field58 = (pfs->field64 + 1) * 8;
    pfs->field5c = pfs->field58 + pfs->field64 * 8;
    result = func_80027330(pfs->field04, pfs->field08, 7, pfs->field2c);
    if (result != 0) {
        return result;
    }
    return 0;
}

/* Adapted leaf 80026E94; see source-provenance.json. */

s32 func_80026E94(ResidentPfs *arg0)
{
    u8 buffer[32];
    s32 status;

    if (arg0->field65 != 0) {
        status = func_8002F640(arg0, 0);
        if (status == 2) {
            status = func_8002F640(arg0, 0);
        }
        if (status != 0) {
            return status;
        }
    }

    status = func_80027330(arg0->field04, arg0->field08, 1, buffer);
    if (status != 0) {
        if (status != 2) {
            return status;
        }
        status = func_80027330(arg0->field04, arg0->field08, 1, buffer);
        if (status != 0) {
            return status;
        }
    }

    return (func_800261B0(arg0->field0c, buffer, 32) != 0) * 2;
}

/* Adapted leaf 80026F4C; see source-provenance.json. */

s32 func_80026F4C(ResidentPfs *arg0, ResidentInode *arg1, u8 arg2, u8 arg3)
{
    u8 sum;
    s32 i;
    s32 offset;
    s32 status;
    u8 *block;

    if (arg2 == 0 && arg3 == D_80036F64 && arg0->field08 == D_80036F60) {
        func_800262C0(&D_800411D0, arg1, 256);
        return 0;
    }

    if (arg0->field65 != 0) {
        status = func_8002F640(arg0, 0);
        if (status != 0) {
            return status;
        }
    }

    offset = 1;
    if (arg3 == 0) {
        offset = arg0->field60;
    }

    if (arg2 == 1) {
        arg1->units[0].bytes.field01 = (u8)func_80026800(offset * 2 + (u8 *)arg1,
                                                  (128 - offset) * 2);
    }

    for (i = 0; i < 8; i++) {
        block = (u8 *)arg1 + i * 32;
        if (arg2 == 1) {
            status = func_80027520(arg0->field04, arg0->field08,
                         arg0->field54 + arg3 * 8 + i, block, 0);
            status = func_80027520(arg0->field04, arg0->field08,
                                  arg0->field58 + arg3 * 8 + i, block, 0);
        } else {
            status = func_80027330(arg0->field04, arg0->field08,
                                  arg0->field54 + arg3 * 8 + i, block);
        }
        if (status != 0) {
            return status;
        }
    }

    if (arg2 == 0) {
        sum = func_80026800((u8 *)&arg1->units[offset], (128 - offset) * 2);
        if (sum != arg1->units[0].bytes.field01) {
            for (i = 0; i < 8; i++) {
                block = (u8 *)arg1 + i * 32;
                status = func_80027330(arg0->field04, arg0->field08,
                             arg0->field58 + arg3 * 8 + i, block);
            }

            sum = func_80026800((u8 *)&arg1->units[offset], (128 - offset) * 2);
            if (sum != arg1->units[0].bytes.field01) {
                return 3;
            }

            for (i = 0; i < 8; i++) {
                block = (u8 *)arg1 + i * 32;
                status = func_80027520(arg0->field04, arg0->field08,
                             arg0->field54 + arg3 * 8 + i, block, 0);
            }
        }
    }

    D_80036F64 = arg3;
    func_800262C0(arg1, &D_800411D0, 256);
    D_80036F60 = arg0->field08;
    return 0;
}
