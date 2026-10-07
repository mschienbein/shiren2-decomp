#ifndef SHIREN2_CONTROLLER_STATUS_H
#define SHIREN2_CONTROLLER_STATUS_H

/* Observed four-byte status records shared by the response helper and wrapper.
 * Original SDK names remain unresolved; this is a measured partial API view.
 */
typedef struct ProbeControllerStatus {
    unsigned short type;
    unsigned char status;
    unsigned char error;
} ProbeControllerStatus;

#endif
