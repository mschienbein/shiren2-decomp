#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Registered client: next link, null-terminated entry table, id, entry count. */
typedef struct Client80131BA0 {
    struct Client80131BA0 *next;
    void **entries;
    u16 id;
    u8 count;
} Client80131BA0;

extern Client80131BA0 *D_80148E70;
extern u32 func_80031F90(u32 mask);

/* Append client to the registry unless its id is already present. */
void func_80131BA0(Client80131BA0 *client) {
    Client80131BA0 **link = &D_80148E70;
    s32 count;
    u32 mask;

    while (*link != 0) {
        if ((*link)->id == client->id) {
            return;
        }
        link = &(*link)->next;
    }
    for (count = 1; client->entries[count] != 0; count++) {
    }
    mask = func_80031F90(1);
    *link = client;
    client->next = 0;
    client->count = count;
    func_80031F90(mask);
}
