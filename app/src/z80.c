#include <z80.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <interface.h>
#include <unistd.h>
#include <z80_priv.h>
#include <time.h>
#include <emulator.h>

typedef enum control_out_bits {
    M1_B = 6,
    MREQ_B = 0,
    IORQ_B = 1,
    RD_B = 2,
    WR_B = 3,
    RFSH_B = 7,
    HALT_B = 5,
    BUSACK_B = 4
}control_out_bits;

typedef enum control_in_bits {
    WAIT_B = 0,
    INTERUPT_B = 1,
    NMI_B = 2,
    BUSRQ_B = 4,
    RESET_B = 3,
    CLOCK_B = 5
}control_in_bits;

void z80_set_pin_state_data(z80_t *cpu, DataT data){
    cpu->pin_state.data = data;
}
DataT z80_get_pin_state_data(z80_t *cpu){
    return cpu->pin_state.data;
}
void z80_set_pin_state_address(z80_t *cpu, AddressT address){
    cpu->pin_state.address = address;
}
AddressT z80_get_pin_state_address(z80_t *cpu){
    return cpu->pin_state.address;
}


Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(m1);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(mreq);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(iorq);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(rd);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(wr);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(rfsh);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(halt);
Z80_PIN_STATE_CONTROL_OUT_CREATE_SETTER_GETTER(busack);

Z80_PIN_STATE_CONTROL_IN_CREATE_SETTER_GETTER(wait);
Z80_PIN_STATE_CONTROL_IN_CREATE_SETTER_GETTER(interupt);
Z80_PIN_STATE_CONTROL_IN_CREATE_SETTER_GETTER(nmi);
Z80_PIN_STATE_CONTROL_IN_CREATE_SETTER_GETTER(busrq);
Z80_PIN_STATE_CONTROL_IN_CREATE_SETTER_GETTER(reset);

SignalStateT getKbit(uint8_t data, uint8_t k){

    bool bit = (data & ((0x01)<<k)) >> k;
    return (bit)? HIGH : LOW;
}

z80_t* z80_init(z80_identity identity)
{
    static size_t id_emu = 0;
    static size_t id_hw = 0;
    z80_t* cpu = NULL;
    cpu = malloc(sizeof(*cpu));
    *cpu = (z80_t){0};
    cpu->clock = 0;
    
    cpu->identity = identity;
if(identity == Z80_HARDWARE){
    cpu->cpu_id = id_hw++;
    cpu->uart_file = serial_init();
} else if(identity == Z80_EMULATOR){
    cpu->cpu_id = id_emu++;
    cpu->uart_file = NULL;
    emulator_init(cpu);
}
#ifdef LOG_PINS
    // char filename[20];
    // sprintf(filename,"log_%lu_%lu.txt",cpu->cpu_id,time(NULL));
    // cpu->log_file = fopen(filename,"w");
    //filename log_cpu_date_time
    char filename[100];
    time_t t;
    time(&t);
    struct tm *ptm = localtime(&t);
    if(identity == Z80_HARDWARE){
        sprintf(filename,"log_cpu_hardware_%lu_%d-%02d-%d_%d:%d:%d.txt",cpu->cpu_id,ptm->tm_mday,ptm->tm_mon+1,ptm->tm_year+1900,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    } else if(identity == Z80_EMULATOR){
        sprintf(filename,"log_cpu_emulator_%lu_%d-%02d-%d_%d:%d:%d.txt",cpu->cpu_id,ptm->tm_mday,ptm->tm_mon+1,ptm->tm_year+1900,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    }
    cpu->log_file = fopen(filename,"w");
    fprintf(cpu->log_file,"Clock\tM1\tMREQ\tIORQ\tRD\tWR\tRFSH\tHALT\tBUSACK\tWAIT\tINT\tNMI\tBUSRQ\tRESET\tADDR\tDATA\tBUS_DATA\n");
#endif
    return cpu;
}

void z80_free(z80_t* cpu){
    free(cpu);
}

void z80_update(z80_t *cpu, DataT bus_data, control_in_s bus_control)
{
    // printf("Clock =%lu\n",cpu->clock);
    // usleep(10);
    
    z80_set_pin_state_control_in_wait(cpu, bus_control.wait);
    z80_set_pin_state_control_in_interupt(cpu, bus_control.interupt);
    z80_set_pin_state_control_in_nmi(cpu, bus_control.nmi);
    z80_set_pin_state_control_in_busrq(cpu, bus_control.busrq);
    z80_set_pin_state_control_in_reset(cpu, bus_control.reset);

    uint8_t data = bus_data;

    if(cpu->identity == Z80_HARDWARE){
    
    uint8_t control = 0;
    
    
    control |= z80_get_pin_state_control_in_wait(cpu)     << WAIT_B; 
    control |= z80_get_pin_state_control_in_interupt(cpu) << INTERUPT_B; 
    control |= z80_get_pin_state_control_in_nmi(cpu)      << NMI_B;  
    control |= z80_get_pin_state_control_in_reset(cpu)    << RESET_B; 
    control |= z80_get_pin_state_control_in_busrq(cpu)    << BUSRQ_B;; 
    control |= (cpu->clock & 1)                           << CLOCK_B;

    putc(control,cpu->uart_file);
    putc(data,cpu->uart_file);


    uint8_t addrH, addrL;
    control = getc(cpu->uart_file);
    addrH = getc(cpu->uart_file);
    addrL = getc(cpu->uart_file);
    data = getc(cpu->uart_file);
    
    AddressT addr;
    addr = addrH << 8 | addrL;

    z80_set_pin_state_data(cpu,data);
    z80_set_pin_state_address(cpu,addr);

    z80_set_pin_state_control_out_mreq(cpu,getKbit(control,MREQ_B));
    z80_set_pin_state_control_out_iorq(cpu,getKbit(control,IORQ_B));
    z80_set_pin_state_control_out_rd(cpu,getKbit(control,RD_B));
    z80_set_pin_state_control_out_wr(cpu,getKbit(control,WR_B));
    z80_set_pin_state_control_out_busack(cpu,getKbit(control,BUSACK_B));
    z80_set_pin_state_control_out_halt(cpu,getKbit(control,HALT_B));
    z80_set_pin_state_control_out_m1(cpu,getKbit(control,M1_B));
    z80_set_pin_state_control_out_rfsh(cpu,getKbit(control,RFSH_B));
} else if(cpu->identity == Z80_EMULATOR){
    if(z80_get_pin_state_control_out_rd(cpu) == LOW){
        z80_set_pin_state_data(cpu,data);
    }
    emulator_tick(cpu);
}

#ifdef LOG_PINS
    fprintf(cpu->log_file,"%5lu/%lu\t%3s\t%5s\t%5s\t%3s\t%3s\t%5s\t%5s\t%7s\t%5s\t%4s\t%4s\t%5s\t%5s\t0x%04x\t0x%02x\t0x%02x\n",
                          (cpu->clock)/2,
                          (cpu->clock)&1,
                          z80_get_pin_state_control_out_m1(cpu)? "":"M1",
                          z80_get_pin_state_control_out_mreq(cpu)? "":"MREQ",
                          z80_get_pin_state_control_out_iorq(cpu)? "":"IORQ",
                          z80_get_pin_state_control_out_rd(cpu)? "":"RD",
                          z80_get_pin_state_control_out_wr(cpu)? "":"WR",
                          z80_get_pin_state_control_out_rfsh(cpu)? "":"RFSH",
                          z80_get_pin_state_control_out_halt(cpu)? "":"HALT",
                          z80_get_pin_state_control_out_busack(cpu)? "":"BUSACK",
                          z80_get_pin_state_control_in_wait(cpu)? "":"WAIT",
                          z80_get_pin_state_control_in_interupt(cpu)? "":"INT",
                          z80_get_pin_state_control_in_nmi(cpu)? "":"NMI",
                          z80_get_pin_state_control_in_busrq(cpu)? "":"BUSRQ",
                          z80_get_pin_state_control_in_reset(cpu)? "":"RESET",
                          z80_get_pin_state_address(cpu),
                          z80_get_pin_state_data(cpu),
                          bus_data);

#endif
cpu->clock++;
}



size_t z80_get_clock(z80_t *cpu){
    return cpu->clock;
}
