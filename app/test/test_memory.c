#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <types.h>
#include <constants.h>

void test_memory_clear();
void test_memory_get_set();
void test_memory_load();

void main_test_memory(){
    printf("TEST MEMORY START ...");
    test_memory_clear();
    test_memory_get_set();
    test_memory_load();
    printf(" END\n");
}


void test_memory_clear(){
    {
        memory_clear();
        bool flag = true;
        AddressT i;
        for (i = 0; i < MEMORY_MAX_SIZE; i++){
            flag &= (0 == memory_get(i));
        }
        assert(flag);
    }
    {
        memory_set(0xdead,0xbe);
        memory_set(MEMORY_ROM_SIZE,0xef);
        memory_clear();
        bool flag = true;
        AddressT i;
        for (i = 0; i < MEMORY_MAX_SIZE; i++){
            flag &= (0 == memory_get(i));
        }
        assert(flag);
    }
}

void test_memory_get_set(){
    {
        memory_clear();
        AddressT addr = 0x1923;
        assert(0 == memory_get(addr));
        DataT x = 0x51;
        memory_set(addr,x);
        assert(x == memory_get(addr));
    }
}

void test_memory_load(){
    {
        memory_clear();
        uint8_t mem[22] = {
            0x21, 0x0a, 0x00, //ld hl, ??
            0x0e, 0x00, //ld c, 0
            0x06, 0x0c, //ld b, 12
            0xed, 0xb3, //otir
            0x76, //halt
            'H','e','l','l','o', ' ','W','o','r','l','d','\n'
        };
        memory_load_rom(mem,22);
        for(uint8_t i=0; i < 22; i++){
            assert(mem[i] == memory_get(i));
        }
    }
}
