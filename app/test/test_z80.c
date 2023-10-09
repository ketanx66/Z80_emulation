#include <stdio.h>
#include <assert.h>
#include <types.h>
#include <constants.h>
#include <z80.h>

void test_setters_getters();

void main_test_z80(){
    printf("TEST Z80 START ...");
    test_setters_getters();
    printf(" END\n");
}


void test_setters_getters(){
    z80_t* cpu = z80_init(Z80_EMULATOR);
    SignalStateT state = LOW;

    z80_set_pin_state_control_in_wait(cpu, state);
    assert(z80_get_pin_state_control_in_wait(cpu) == state);
    z80_set_pin_state_control_in_interupt(cpu, state);
    assert(z80_get_pin_state_control_in_interupt(cpu) == state);
    z80_set_pin_state_control_in_nmi(cpu, state);
    assert(z80_get_pin_state_control_in_nmi(cpu) == state);
    z80_set_pin_state_control_in_busrq(cpu, state);
    assert(z80_get_pin_state_control_in_busrq(cpu) == state);
    z80_set_pin_state_control_in_reset(cpu, state);
    assert(z80_get_pin_state_control_in_reset(cpu) == state);
    
    z80_set_pin_state_control_out_m1(cpu, state);
    assert(z80_get_pin_state_control_out_m1(cpu) == state);
    z80_set_pin_state_control_out_mreq(cpu, state);
    assert(z80_get_pin_state_control_out_mreq(cpu) == state);
    z80_set_pin_state_control_out_iorq(cpu, state);
    assert(z80_get_pin_state_control_out_iorq(cpu) == state);
    z80_set_pin_state_control_out_rd(cpu, state);
    assert(z80_get_pin_state_control_out_rd(cpu) == state);
    z80_set_pin_state_control_out_wr(cpu, state);
    assert(z80_get_pin_state_control_out_wr(cpu) == state);
    z80_set_pin_state_control_out_rfsh(cpu, state);
    assert(z80_get_pin_state_control_out_rfsh(cpu) == state);
    z80_set_pin_state_control_out_halt(cpu, state);
    assert(z80_get_pin_state_control_out_halt(cpu) == state);
    z80_set_pin_state_control_out_busack(cpu, state);
    assert(z80_get_pin_state_control_out_busack(cpu) == state);

    state = HIGH;

    z80_set_pin_state_control_in_wait(cpu, state);
    assert(z80_get_pin_state_control_in_wait(cpu) == state);
    z80_set_pin_state_control_in_interupt(cpu, state);
    assert(z80_get_pin_state_control_in_interupt(cpu) == state);
    z80_set_pin_state_control_in_nmi(cpu, state);
    assert(z80_get_pin_state_control_in_nmi(cpu) == state);
    z80_set_pin_state_control_in_busrq(cpu, state);
    assert(z80_get_pin_state_control_in_busrq(cpu) == state);
    z80_set_pin_state_control_in_reset(cpu, state);
    assert(z80_get_pin_state_control_in_reset(cpu) == state);
    
    z80_set_pin_state_control_out_m1(cpu, state);
    assert(z80_get_pin_state_control_out_m1(cpu) == state);
    z80_set_pin_state_control_out_mreq(cpu, state);
    assert(z80_get_pin_state_control_out_mreq(cpu) == state);
    z80_set_pin_state_control_out_iorq(cpu, state);
    assert(z80_get_pin_state_control_out_iorq(cpu) == state);
    z80_set_pin_state_control_out_rd(cpu, state);
    assert(z80_get_pin_state_control_out_rd(cpu) == state);
    z80_set_pin_state_control_out_wr(cpu, state);
    assert(z80_get_pin_state_control_out_wr(cpu) == state);
    z80_set_pin_state_control_out_rfsh(cpu, state);
    assert(z80_get_pin_state_control_out_rfsh(cpu) == state);
    z80_set_pin_state_control_out_halt(cpu, state);
    assert(z80_get_pin_state_control_out_halt(cpu) == state);
    z80_set_pin_state_control_out_busack(cpu, state);
    assert(z80_get_pin_state_control_out_busack(cpu) == state);

    z80_free(cpu);
}
