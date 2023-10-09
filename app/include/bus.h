#ifndef BUS_H
#define BUS_H

#include <types.h>
#include <z80.h>

void bus_clear();
void bus_data_write(DataT data);
DataT bus_data_read();
void bus_address_write(AddressT address);
AddressT bus_address_read();

void bus_update(z80_t *cpu);
control_in_s bus_get_control_in();

#define string_value_(x) #x
#define string_value(x) string_value_(x)

#ifdef DEBUG
#define CONTROL_CHANGE_PRINT(name) \
    if(_bus.system_control.name != state) \
    { \
        const char* str = string_value(name); \
        printf("Changed %s to %u\n",str,state); \
    } \

#else 
#define CONTROL_CHANGE_PRINT(x)
#endif

#define BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(name) \
    void bus_set_system_control_##name(SignalStateT state) \
    { \
        CONTROL_CHANGE_PRINT(name) \
        _bus.system_control.name = state; \
    } \
    SignalStateT bus_get_system_control_##name() \
    { \
        return _bus.system_control.name; \
    }

#define BUS_CPU_CONTROL_CREATE_SETTER_GETTER(name) \
    void bus_set_cpu_control_##name(SignalStateT state) \
    { \
        _bus.cpu_control.name = state; \
    } \
    SignalStateT bus_get_cpu_control_##name() \
    { \
        return _bus.cpu_control.name; \
    }

#define BUS_CPU_BUS_CONTROL_CREATE_SETTER_GETTER(name) \
    void bus_set_cpu_bus_control_##name(SignalStateT state) \
    { \
        _bus.cpu_bus_control.name = state; \
    } \
    SignalStateT bus_get_cpu_bus_control_##name() \
    { \
        return _bus.cpu_bus_control.name; \
    }


void bus_set_system_control_m1(SignalStateT state);
void bus_set_system_control_mreq(SignalStateT state);
void bus_set_system_control_iorq(SignalStateT state);
void bus_set_system_control_rd(SignalStateT state);
void bus_set_system_control_wr(SignalStateT state);
void bus_set_system_control_rfsh(SignalStateT state);
void bus_set_cpu_control_halt(SignalStateT state);
void bus_set_cpu_control_wait(SignalStateT state);
void bus_set_cpu_control_interupt(SignalStateT state);
void bus_set_cpu_control_nmi(SignalStateT state);
void bus_set_cpu_control_reset(SignalStateT state);
void bus_set_cpu_bus_control_busrq(SignalStateT state);
void bus_set_cpu_bus_control_busack(SignalStateT state);

SignalStateT bus_get_system_control_m1();
SignalStateT bus_get_system_control_mreq();
SignalStateT bus_get_system_control_iorq();
SignalStateT bus_get_system_control_rd();
SignalStateT bus_get_system_control_wr();
SignalStateT bus_get_system_control_rfsh();
SignalStateT bus_get_cpu_control_halt();
SignalStateT bus_get_cpu_control_wait();
SignalStateT bus_get_cpu_control_interupt();
SignalStateT bus_get_cpu_control_nmi();
SignalStateT bus_get_cpu_control_reset();
SignalStateT bus_get_cpu_bus_control_busrq();
SignalStateT bus_get_cpu_bus_control_busack();

#endif //BUS_H
