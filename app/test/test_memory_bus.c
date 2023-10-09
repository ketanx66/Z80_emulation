#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <bus.h>
#include <constants.h>

void test_memory_bus_write();
void test_memory_bus_read();

void main_test_memory_bus(){
    printf("TEST MEMORY/BUS START ...");
    test_memory_bus_write();
    test_memory_bus_read();
    printf(" END\n");
}


void test_memory_bus_write(){
    {//RAM
        memory_clear();
        AddressT addr = MEMORY_ROM_SIZE + (0xbadd % MEMORY_RAM_SIZE);
        const DataT data = 0x14;
        bus_address_write(addr);
        bus_data_write(data);
        memory_write();
        assert(data == memory_get(addr));
        addr = MEMORY_ROM_SIZE + (0x0001 % MEMORY_RAM_SIZE);
        bus_address_write(addr);
        memory_write();
        assert(data == memory_get(addr));
    }
    {//RAM
        memory_clear();
        const AddressT addr = MEMORY_ROM_SIZE + (0xf7ab % MEMORY_RAM_SIZE);
        const DataT data1 = 0x14;
        const DataT data2 = 0xa5;
        bus_address_write(addr);
        bus_data_write(data1);
        memory_write();
        assert(data1 == memory_get(addr));
        bus_data_write(data2);
        memory_write();
        assert(data2 == memory_get(addr));
    }
    {//RAM
        memory_clear();
        const AddressT addr1 = MEMORY_ROM_SIZE + (0xbadd % MEMORY_RAM_SIZE);
        const AddressT addr2 = MEMORY_ROM_SIZE + (0x1234 % MEMORY_RAM_SIZE);
        const DataT data1 = 0x14;
        const DataT data2 = 0x52;
        bus_address_write(addr1);
        bus_data_write(data1);
        memory_write();
        bus_address_write(addr2);
        bus_data_write(data2);
        memory_write();
        assert(data1 == memory_get(addr1));
        assert(data2 == memory_get(addr2));
    }
    {//ROM
        memory_clear();
        const AddressT addr = 0xbeef % MEMORY_ROM_SIZE;
        const DataT data = 0xf5;
        bus_address_write(addr);
        bus_data_write(data);
        memory_write();
        assert(0 == memory_get(addr));
    }
}

void test_memory_bus_read(){
    {//RAM
        memory_clear();
        const AddressT addr = MEMORY_ROM_SIZE + (0x7fa6 % MEMORY_RAM_SIZE);
        const DataT data = 0x5a;
        memory_set(addr,data);
        bus_address_write(addr);
        memory_read();
        assert(data == bus_data_read());
    }
    {//RAM
        memory_clear();
        const AddressT addr1 = MEMORY_ROM_SIZE + (0xbad2 % MEMORY_RAM_SIZE);
        const AddressT addr2 = MEMORY_ROM_SIZE + (0x024a % MEMORY_RAM_SIZE);
        const DataT data1 = 0x14;
        const DataT data2 = 0x52;
        memory_set(addr1,data1);
        memory_set(addr2,data2);

        bus_address_write(addr1);
        memory_read();
        assert(data1 == bus_data_read());
        bus_address_write(addr2);
        memory_read();
        assert(data2 == bus_data_read());
    }
    {//ROM
        memory_clear();
        const AddressT addr = 0x5e1d % MEMORY_ROM_SIZE;
        const DataT data = 0xc5;
        memory_set(addr,data);
        bus_address_write(addr);
        memory_read();
        assert(data == bus_data_read());
    }
}