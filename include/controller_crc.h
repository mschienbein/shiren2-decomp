#ifndef PRIVATE_CONTROLLER_CRC_H
#define PRIVATE_CONTROLLER_CRC_H

typedef unsigned int u32;
typedef unsigned char u8;

typedef char controller_crc_word_size[(sizeof(u32) == 4) ? 1 : -1];
typedef char controller_crc_byte_size[(sizeof(u8) == 1) ? 1 : -1];

/* Observed o32 argument/return views; historical SDK declarations are unknown. */
u8 func_80027DB0(u32 address);
u8 func_80027E1C(const u8 *data);

#endif
