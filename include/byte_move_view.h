#ifndef SHIREN2_BYTE_MOVE_VIEW_H
#define SHIREN2_BYTE_MOVE_VIEW_H

/* Partial view only: the original reads a byte pointer at offset +8.
 * The owner's allocation, the prefix fields and the byte buffer's size are
 * unresolved. Byte stores can alias this record, including its pointer. */
typedef struct ByteMoveView {
    unsigned char unresolved_prefix[8];
    unsigned char *bytes;
} ByteMoveView;

#endif
