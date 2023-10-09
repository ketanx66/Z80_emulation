#include <bus.h>
#include <stdio.h>

typedef struct Bus{
    AddressT address;
    DataT data;
    struct system_control
    {
        ControlT m1      : 1;
        ControlT mreq    : 1;
        ControlT iorq    : 1;
        ControlT rd      : 1;
        ControlT wr      : 1;
        ControlT rfsh    : 1;
        ControlT padding : 2;
    }system_control;
    struct cpu_control
    {
        ControlT halt     : 1;
        ControlT wait     : 1;
        ControlT interupt : 1;
        ControlT nmi      : 1;
        ControlT reset    : 1;
        ControlT padding  : 3;
    }cpu_control;
    struct cpu_bus_control
    {
        ControlT busrq   : 1;
        ControlT busack  : 1;
        ControlT padding : 6;
    }cpu_bus_control;
} Bus;

Bus _bus;

void bus_clear(){
    _bus = (Bus){
        .system_control = {
            .m1 = HIGH,
            .mreq = HIGH,
            .iorq = HIGH,
            .rd = HIGH,
            .wr = HIGH,
            .rfsh = HIGH
        },
        .cpu_control = {
            .halt = HIGH,
            .wait = HIGH,
            .interupt = HIGH,
            .nmi = HIGH,
            .reset = LOW,
            .padding = HIGH,
        },
        .cpu_bus_control = {
            .busrq = HIGH,
            .busack = HIGH
        }
    };
}

void bus_data_write(DataT data){
    _bus.data = data;
}

DataT bus_data_read(){
    return _bus.data;
}

void bus_address_write(AddressT address){
    _bus.address = address;
}

AddressT bus_address_read(){
    return _bus.address;
}

BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(m1);
BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(mreq);
BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(iorq);
BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(rd);
BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(wr);
BUS_SYSTEM_CONTROL_CREATE_SETTER_GETTER(rfsh);

BUS_CPU_CONTROL_CREATE_SETTER_GETTER(halt);
BUS_CPU_CONTROL_CREATE_SETTER_GETTER(wait);
BUS_CPU_CONTROL_CREATE_SETTER_GETTER(interupt);
BUS_CPU_CONTROL_CREATE_SETTER_GETTER(nmi);
BUS_CPU_CONTROL_CREATE_SETTER_GETTER(reset);

BUS_CPU_BUS_CONTROL_CREATE_SETTER_GETTER(busrq);
BUS_CPU_BUS_CONTROL_CREATE_SETTER_GETTER(busack);

control_in_s bus_get_control_in(){
    return (control_in_s){
        .wait = bus_get_cpu_control_wait(),
        .interupt = bus_get_cpu_control_interupt(),
        .nmi = bus_get_cpu_control_nmi(),
        .busrq = bus_get_cpu_bus_control_busrq(),
        .reset = bus_get_cpu_control_reset()
    };
}

void bus_update(z80_t *cpu){
    bus_set_system_control_m1(z80_get_pin_state_control_out_m1(cpu));
    bus_set_system_control_mreq(z80_get_pin_state_control_out_mreq(cpu));
    bus_set_system_control_iorq(z80_get_pin_state_control_out_iorq(cpu));
    bus_set_system_control_rd(z80_get_pin_state_control_out_rd(cpu));
    bus_set_system_control_wr(z80_get_pin_state_control_out_wr(cpu));
    bus_set_system_control_rfsh(z80_get_pin_state_control_out_rfsh(cpu));
    bus_set_cpu_control_halt(z80_get_pin_state_control_out_halt(cpu));
    bus_set_cpu_bus_control_busack(z80_get_pin_state_control_out_busack(cpu));

    if(bus_get_system_control_rd() == LOW){
        bus_address_write(z80_get_pin_state_address(cpu));
    }
    if(bus_get_system_control_wr() == LOW){
        bus_address_write(z80_get_pin_state_address(cpu));
        if(bus_get_system_control_mreq() == LOW ||
           bus_get_system_control_iorq() == LOW){
          bus_data_write(z80_get_pin_state_data(cpu));
      }
    }
}