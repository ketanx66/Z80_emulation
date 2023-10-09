#include <stdio.h>
#include <bus.h>
#include <assert.h>

void test_bus_clear();
void test_bus_write_data_read_data();
void test_bus_write_address_read_address();
void test_bus_get_set();

void main_test_bus(){
    printf("TEST BUS START ...");
    test_bus_clear();
    test_bus_get_set();
    test_bus_write_data_read_data();
    test_bus_write_address_read_address();
    printf(" END\n");
}

void test_bus_clear(){
    {
        bus_clear();
        assert(0 == bus_data_read());
        assert(0 == bus_address_read());
        assert(HIGH == bus_get_system_control_m1());
        assert(HIGH == bus_get_system_control_mreq());
        assert(HIGH == bus_get_system_control_iorq());
        assert(HIGH == bus_get_system_control_rd());
        assert(HIGH == bus_get_system_control_wr());
        assert(HIGH == bus_get_system_control_rfsh());
        assert(HIGH == bus_get_cpu_control_halt());
        assert(HIGH == bus_get_cpu_control_wait());
        assert(HIGH == bus_get_cpu_control_interupt());
        assert(HIGH == bus_get_cpu_control_nmi());
        assert(LOW  == bus_get_cpu_control_reset());
        assert(HIGH == bus_get_cpu_bus_control_busrq());
        assert(HIGH == bus_get_cpu_bus_control_busack());
    }
    {
        bus_clear();
        DataT x;
        x = 0xde;
        bus_data_write(x);
        bus_clear();
        assert(0 == bus_data_read());
    }
    {
        bus_clear();
        AddressT x;
        x = 0xdead;
        bus_address_write(x);
        bus_clear();
        assert(0 == bus_data_read());
    }
}

void test_bus_write_data_read_data(){
    {
        bus_clear();
        DataT x;
        x = 0xad;
        bus_data_write(x);
        assert(x == bus_data_read());
    }
    {
        bus_clear();
        DataT x,y;
        x = 0xbe;
        y = 0xef;
        bus_data_write(x);
        bus_data_write(y);
        assert(y == bus_data_read());
    }
}

void test_bus_write_address_read_address(){
    {
        bus_clear();
        AddressT x;
        x = 0xbeef;
        bus_address_write(x);
        assert(x == bus_address_read());
    }
    {
        bus_clear();
        AddressT x,y;
        x = 0xdead;
        y = 0xbeaf;
        bus_address_write(x);
        bus_address_write(y);
        assert(y == bus_address_read());
    }
}

void test_bus_get_set(){
    bus_clear();
    {
        bus_set_system_control_m1(HIGH);
        assert(bus_get_system_control_m1() == HIGH);
        bus_set_system_control_m1(LOW);
        assert(bus_get_system_control_m1() == LOW);
        bus_set_system_control_m1(HIGH);
        assert(bus_get_system_control_m1() == HIGH);
    }
    {
        bus_set_system_control_mreq(HIGH);
        assert(bus_get_system_control_mreq() == HIGH);
        bus_set_system_control_mreq(LOW);
        assert(bus_get_system_control_mreq() == LOW);
        bus_set_system_control_mreq(HIGH);
        assert(bus_get_system_control_mreq() == HIGH);
    }
    {
        bus_set_system_control_iorq(HIGH);
        assert(bus_get_system_control_iorq() == HIGH);
        bus_set_system_control_iorq(LOW);
        assert(bus_get_system_control_iorq() == LOW);
        bus_set_system_control_iorq(HIGH);
        assert(bus_get_system_control_iorq() == HIGH);
    }
    {
        bus_set_system_control_rd(HIGH);
        assert(bus_get_system_control_rd() == HIGH);
        bus_set_system_control_rd(LOW);
        assert(bus_get_system_control_rd() == LOW);
        bus_set_system_control_rd(HIGH);
        assert(bus_get_system_control_rd() == HIGH);
    }
    {
        bus_set_system_control_wr(HIGH);
        assert(bus_get_system_control_wr() == HIGH);
        bus_set_system_control_wr(LOW);
        assert(bus_get_system_control_wr() == LOW);
        bus_set_system_control_wr(HIGH);
        assert(bus_get_system_control_wr() == HIGH);
    }
    {
        bus_set_system_control_rfsh(HIGH);
        assert(bus_get_system_control_rfsh() == HIGH);
        bus_set_system_control_rfsh(LOW);
        assert(bus_get_system_control_rfsh() == LOW);
        bus_set_system_control_rfsh(HIGH);
        assert(bus_get_system_control_rfsh() == HIGH);
    }
    {
        bus_set_cpu_control_halt(HIGH);
        assert(bus_get_cpu_control_halt() == HIGH);
        bus_set_cpu_control_halt(LOW);
        assert(bus_get_cpu_control_halt() == LOW);
        bus_set_cpu_control_halt(HIGH);
        assert(bus_get_cpu_control_halt() == HIGH);
    }
    {
        bus_set_cpu_control_wait(HIGH);
        assert(bus_get_cpu_control_wait() == HIGH);
        bus_set_cpu_control_wait(LOW);
        assert(bus_get_cpu_control_wait() == LOW);
        bus_set_cpu_control_wait(HIGH);
        assert(bus_get_cpu_control_wait() == HIGH);
    }
    {
        bus_set_cpu_control_interupt(HIGH);
        assert(bus_get_cpu_control_interupt() == HIGH);
        bus_set_cpu_control_interupt(LOW);
        assert(bus_get_cpu_control_interupt() == LOW);
        bus_set_cpu_control_interupt(HIGH);
        assert(bus_get_cpu_control_interupt() == HIGH);
    }
    {
        bus_set_cpu_control_nmi(HIGH);
        assert(bus_get_cpu_control_nmi() == HIGH);
        bus_set_cpu_control_nmi(LOW);
        assert(bus_get_cpu_control_nmi() == LOW);
         bus_set_cpu_control_nmi(HIGH);
        assert(bus_get_cpu_control_nmi() == HIGH);
    }
    {
        bus_set_cpu_control_reset(HIGH);
        assert(bus_get_cpu_control_reset() == HIGH);
        bus_set_cpu_control_reset(LOW);
        assert(bus_get_cpu_control_reset() == LOW);
        bus_set_cpu_control_reset(HIGH);
        assert(bus_get_cpu_control_reset() == HIGH);
    }
    {
        bus_set_cpu_bus_control_busrq(HIGH);
        assert(bus_get_cpu_bus_control_busrq() == HIGH);
        bus_set_cpu_bus_control_busrq(LOW);
        assert(bus_get_cpu_bus_control_busrq() == LOW);
        bus_set_cpu_bus_control_busrq(HIGH);
        assert(bus_get_cpu_bus_control_busrq() == HIGH);
    }
    {
        bus_set_cpu_bus_control_busack(HIGH);
        assert(bus_get_cpu_bus_control_busack() == HIGH);
        bus_set_cpu_bus_control_busack(LOW);
        assert(bus_get_cpu_bus_control_busack() == LOW);
        bus_set_cpu_bus_control_busack(HIGH);
        assert(bus_get_cpu_bus_control_busack() == HIGH);
    }
}