#ifndef Z80_H
#define Z80_H

#include <types.h>
#include <stdio.h>
#include <stddef.h>
#include <z80_priv.h>

z80_t* z80_init(z80_identity identity);
void z80_free();
void z80_update(z80_t *cpu, DataT data, control_in_s bus_control);
size_t z80_get_clock(z80_t *cpu);

void z80_set_pin_state_data(z80_t *cpu, DataT data);
DataT z80_get_pin_state_data(z80_t *cpu);
void z80_set_pin_state_address(z80_t *cpu, AddressT address);
AddressT z80_get_pin_state_address(z80_t *cpu);


#define Z80_PIN_STATE_CONTROL_IN_CREATE_SETTER_GETTER(name) \
    void z80_set_pin_state_control_in_##name(z80_t *cpu, SignalStateT state) \
    { \
        cpu->pin_state.control_in.name = state; \
    } \
    SignalStateT z80_get_pin_state_control_in_##name(z80_t *cpu) \
    { \
        return cpu->pin_state.control_in.name; \
    }

void z80_set_pin_state_control_in_wait(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_in_interupt(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_in_nmi(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_in_busrq(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_in_reset(z80_t *cpu, SignalStateT state);
SignalStateT z80_get_pin_state_control_in_wait(z80_t *cpu);
SignalStateT z80_get_pin_state_control_in_interupt(z80_t *cpu);
SignalStateT z80_get_pin_state_control_in_nmi(z80_t *cpu);
SignalStateT z80_get_pin_state_control_in_busrq(z80_t *cpu);
SignalStateT z80_get_pin_state_control_in_reset(z80_t *cpu);


#define Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(name) \
    void z80_set_pin_state_control_out_##name(z80_t *cpu, SignalStateT state) \
    { \
        cpu->pin_state.control_out.name = state; \
    } \
    SignalStateT z80_get_pin_state_control_out_##name(z80_t *cpu) \
    { \
        return cpu->pin_state.control_out.name; \
    }

void z80_set_pin_state_control_out_m1(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_mreq(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_iorq(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_rd(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_wr(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_rfsh(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_halt(z80_t *cpu, SignalStateT state);
void z80_set_pin_state_control_out_busack(z80_t *cpu, SignalStateT state);
SignalStateT z80_get_pin_state_control_out_m1(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_mreq(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_iorq(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_rd(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_wr(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_rfsh(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_halt(z80_t *cpu);
SignalStateT z80_get_pin_state_control_out_busack(z80_t *cpu);

#endif //Z80_H
