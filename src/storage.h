/*
    Copyright 2017 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#ifndef STORAGE_H_
#define STORAGE_H_
#include <stdint.h>
#include "frsky.h"

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


#endif  // STORAGE_H_
