#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>
#include <constants.h>
#include <stddef.h>

void memory_clear();
void memory_clear_ram();
void memory_load_rom(DataT* src, size_t size);
void memory_load_ram(DataT* src, size_t size);
void memory_set(AddressT address, DataT data);
DataT memory_get(AddressT address);

void memory_dump(char* filename);
void memory_load(char* filename);

void memory_write();
void memory_read();

#endif //MEMORY_H
