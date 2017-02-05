#ifndef __STORAGE_H__
#define __STORAGE_H__
#include <stdint.h>
#include "frsky.h"
// #include "cc2510fx.h"

#define STORAGE_VERSION_ID 0x01

void storage_init(void);
void storage_write_to_flash(void);
void storage_read_from_flash(void);


// our storage struct contains all data that has to be stored on flash
typedef struct {
    // version id
    uint8_t version;
    // persistent storage for frsky
    uint8_t frsky_txid[2];
    uint8_t frsky_hop_table[FRSKY_HOPTABLE_SIZE];
    int8_t  frsky_freq_offset;
    // add further data here...
} STORAGE_DESC;

extern EXTERNAL_MEMORY STORAGE_DESC storage;


#endif
