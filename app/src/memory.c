#include <memory.h>
#include <bus.h>
#include <stdlib.h>
#include <constants.h>
#include <stdio.h>

DataT _mem[MEMORY_MAX_SIZE];

void memory_clear_rom(){
    AddressT addr;
    for(addr=0; addr<MEMORY_ROM_SIZE; addr++){
        _mem[addr] = 0;
    }
}

void memory_clear_ram(){
    AddressT addr;
    for(addr=MEMORY_ROM_SIZE; addr<MEMORY_ROM_SIZE+MEMORY_RAM_SIZE; addr++){
        _mem[addr] = 0;
    }
}

void memory_clear(){
    memory_clear_rom();
    memory_clear_ram();
}

void memory_load_rom(DataT* src, size_t size){
    unsigned int i;
    for(i=0; i<size && i<MEMORY_ROM_SIZE; i++){
        _mem[i] = src[i];
    }
}

void memory_load_ram(DataT* src, size_t size){
    unsigned int i;
    for(i=MEMORY_ROM_SIZE; i<size && i<MEMORY_RAM_SIZE ; i++){
        _mem[i] = src[i];
    }
}

inline void memory_set(AddressT address, DataT data){
    if(address > MEMORY_MAX_SIZE){
        printf("memory_set: address out of bounds\n");
        exit(1);
    }
    _mem[address] = data;
}
inline DataT memory_get(AddressT address){
    if(address > MEMORY_MAX_SIZE){
        printf("memory_get: address out of bounds\n");
        exit(1);
    }
    return _mem[address];
}

void memory_dump(char* filename){
    FILE* fp = fopen(filename,"wb");
    fwrite(_mem,sizeof(DataT),MEMORY_MAX_SIZE,fp);
    fclose(fp);
}
void memory_load(char* filename){
    FILE* fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("memory_load: file not found\n");
        exit(1);
    }
    size_t bytes_read = fread(_mem,sizeof(DataT),MEMORY_MAX_SIZE,fp);
    for(;bytes_read<MEMORY_MAX_SIZE;bytes_read++){
        _mem[bytes_read] = 0;
    }
    fclose(fp);
}



void memory_write(){
    AddressT addr = bus_address_read();
    if(addr > MEMORY_ROM_SIZE){
        memory_set(addr,bus_data_read());
    }
    // printf("memory_write: addr: %04x, data: %02x\n",addr,bus_data_read());
}
void memory_read(){
    bus_data_write(memory_get(bus_address_read()));
}

