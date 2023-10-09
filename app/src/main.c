#ifndef TEST_BUILD

#include <stdio.h>
#include <bus.h>
#include <memory.h>
#include <z80.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

uint8_t mem[27] = {
0x21, 0x0e, 0x00,//  ; LD HL, 0x000C
0x11, 0x00, 0x80,//  ; LD DE, 0x8000
0x01, 0x0C, 0x00,//   ; LD BC, 0x000c
0xED, 0xB0,//      ; LDIR
0x00,0x00,0x76,//         ; HALT 
'H','e','l','l','o', ' ','W','o','r','l','d','\n'
};

#define PRINT_STATE_SIGNATURE() printf("Clock\tM1\tMREQ\tIORQ\tRD\tWR\tRFSH\tHALT\tBUSACK\tWAIT\tINT\tNMI\tBUSRQ\tRESET\tADDR\tDATA\tBUS_DATA\n");

#define PRINT_STATE(cpu) printf("%5lu/%lu\t%3s\t%5s\t%5s\t%3s\t%3s\t%5s\t%5s\t%7s\t%5s\t%4s\t%4s\t%5s\t%5s\t0x%04x\t0x%02x\t0x%02x\n",\
                          (z80_get_clock(cpu)-1)/2,\
                          (z80_get_clock(cpu)-1)&1,\
                          z80_get_pin_state_control_out_m1(cpu)? "":"M1",\
                          z80_get_pin_state_control_out_mreq(cpu)? "":"MREQ",\
                          z80_get_pin_state_control_out_iorq(cpu)? "":"IORQ",\
                          z80_get_pin_state_control_out_rd(cpu)? "":"RD",\
                          z80_get_pin_state_control_out_wr(cpu)? "":"WR",\
                          z80_get_pin_state_control_out_rfsh(cpu)? "":"RFSH",\
                          z80_get_pin_state_control_out_halt(cpu)? "":"HALT",\
                          z80_get_pin_state_control_out_busack(cpu)? "":"BUSACK",\
                          bus_get_cpu_control_wait()? "":"WAIT",\
                          bus_get_cpu_control_interupt()? "":"INT",\
                          bus_get_cpu_control_nmi()? "":"NMI",\
                          bus_get_cpu_bus_control_busrq()? "":"BUSRQ",\
                          bus_get_cpu_control_reset()? "":"RESET",\
                          z80_get_pin_state_address(cpu),\
                          z80_get_pin_state_data(cpu),\
                          bus_data_read())

#define PRINT_STATE_NO_CLOCK(cpu) printf("       \t%3s\t%5s\t%5s\t%3s\t%3s\t%5s\t%5s\t%7s\t%5s\t%4s\t%4s\t%5s\t%5s\t0x%04x\t0x%02x\t0x%02x\n",\
                          z80_get_pin_state_control_out_m1(cpu)? "":"M1",\
                          z80_get_pin_state_control_out_mreq(cpu)? "":"MREQ",\
                          z80_get_pin_state_control_out_iorq(cpu)? "":"IORQ",\
                          z80_get_pin_state_control_out_rd(cpu)? "":"RD",\
                          z80_get_pin_state_control_out_wr(cpu)? "":"WR",\
                          z80_get_pin_state_control_out_rfsh(cpu)? "":"RFSH",\
                          z80_get_pin_state_control_out_halt(cpu)? "":"HALT",\
                          z80_get_pin_state_control_out_busack(cpu)? "":"BUSACK",\
                          bus_get_cpu_control_wait()? "":"WAIT",\
                          bus_get_cpu_control_interupt()? "":"INT",\
                          bus_get_cpu_control_nmi()? "":"NMI",\
                          bus_get_cpu_bus_control_busrq()? "":"BUSRQ",\
                          bus_get_cpu_control_reset()? "":"RESET",\
                          z80_get_pin_state_address(cpu),\
                          z80_get_pin_state_data(cpu),\
                          bus_data_read())

bool compare_pin_state(z80_t* cpu1, z80_t* cpu2){
    if(bus_get_cpu_control_reset() == LOW ||
       bus_get_cpu_bus_control_busack() == LOW){
        return true;
    }
    AddressT addr1 = z80_get_pin_state_address(cpu1);
    AddressT addr2 = z80_get_pin_state_address(cpu2);
    DataT data1 = z80_get_pin_state_data(cpu1);
    DataT data2 = z80_get_pin_state_data(cpu2);
    bool m1_1 = z80_get_pin_state_control_out_m1(cpu1);
    bool m1_2 = z80_get_pin_state_control_out_m1(cpu2);
    bool mreq_1 = z80_get_pin_state_control_out_mreq(cpu1);
    bool mreq_2 = z80_get_pin_state_control_out_mreq(cpu2);
    bool iorq_1 = z80_get_pin_state_control_out_iorq(cpu1);
    bool iorq_2 = z80_get_pin_state_control_out_iorq(cpu2);
    bool rd_1 = z80_get_pin_state_control_out_rd(cpu1);
    bool rd_2 = z80_get_pin_state_control_out_rd(cpu2);
    bool wr_1 = z80_get_pin_state_control_out_wr(cpu1);
    bool wr_2 = z80_get_pin_state_control_out_wr(cpu2);
    bool rfsh_1 = z80_get_pin_state_control_out_rfsh(cpu1);
    bool rfsh_2 = z80_get_pin_state_control_out_rfsh(cpu2);
    bool halt_1 = z80_get_pin_state_control_out_halt(cpu1);
    bool halt_2 = z80_get_pin_state_control_out_halt(cpu2);

    if(addr1 != addr2 ||
       data1 != data2 ||
       m1_1 != m1_2 ||
       mreq_1 != mreq_2 ||
       iorq_1 != iorq_2 ||
       rd_1 != rd_2 ||
       wr_1 != wr_2 ||
       rfsh_1 != rfsh_2 ||
       halt_1 != halt_2){
        return false;
    }
    return true;
}

int main(int argc, char** argv){
    bool step_mode = false;
    bool load_file = false;
    char* filename;
    bool help = false;
    bool hardware = false;
    bool compare_mode = false;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-s")==0){
            step_mode = true;
        }else if(strcmp(argv[i],"-l")==0){
            load_file = true;
            filename = argv[i+1];
        }else if(strcmp(argv[i],"-h")==0){
            help = true;
        }else if(strcmp(argv[i],"-hw")==0){
            hardware = true;
        }else if(strcmp(argv[i],"-c")==0){
            compare_mode = true;
        }
    }

    if(help){
        printf("Usage: %s [options]\n", argv[0]);
        printf("Options:\n");
        printf("\t-s : step mode\n\t\tStep through execution half-cycle at the time. Toogle control signals. Press h for help\n");
        printf("\t-l <filename> : load file\n\t\tLoad binary file for the Z80 to work on.\n");
        printf("\t-h : help\n\t\tPrint this help message\n");
        printf("\t-hw : hardware Z80\n\t\tRun physical Z80 through Arduino link. Default is to run Z80 emulator.\n");
        printf("\t-c : compare mode\n\t\tCompare output from emulator and physical Z80. Dose not require -hw\n");
        return 0;
    }


    printf("System start\n");

    bus_clear();
    if(load_file){
        memory_load(filename);
    }else{
        memory_load_rom(mem,27);
    }
   

    
    if(compare_mode){
        z80_t* cpu_emu = z80_init(Z80_EMULATOR);
        z80_t* cpu_hw = z80_init(Z80_HARDWARE);
        
        if(step_mode){
            struct termios termios;
            tcgetattr(STDIN_FILENO, &termios);
            termios.c_lflag &=  ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
            z80_clock_t clock = z80_get_clock(cpu_hw);

            printf("Step mode\n");
            printf("Press h for help\n");
            PRINT_STATE_SIGNATURE();
            PRINT_STATE_NO_CLOCK(cpu_hw);
            char c;
            bool change_control = false;
            while((c = getchar()) != 'q'){
                clock = z80_get_clock(cpu_hw);
                switch(c){
                    case 'h':
                        printf("\n\n\nh: help\n");
                        printf("w: toggle WAIT\n");
                        printf("i: toggle INT\n");
                        printf("n: toggle NMI\n");
                        printf("r: toggle RESET\n");
                        printf("b: toggle BUSRQ\n");
                        printf("s: half_step\n");
                        printf("f: emulator flags");
                        printf("q: quit\n");
                        printf("Clock\tM1\tMREQ\tIORQ\tRD\tWR\tRFSH\tHALT\tBUSACK\tWAIT\tINT\tNMI\tBUSRQ\tRESET\tADDR\tDATA\tBUS_DATA\n");
                        break;
                    case 'w':
                        bus_set_cpu_control_wait(!bus_get_cpu_control_wait());
                        change_control = true;
                        break;
                    case 'i':
                        bus_set_cpu_control_interupt(!bus_get_cpu_control_interupt());
                        change_control = true;
                        break;
                    case 'n':
                        bus_set_cpu_control_nmi(!bus_get_cpu_control_nmi());
                        change_control = true;
                        break;
                    case 'r':
                        bus_set_cpu_control_reset(!bus_get_cpu_control_reset());
                        change_control = true;
                        break;
                    case 'b':
                        bus_set_cpu_bus_control_busrq(!bus_get_cpu_bus_control_busrq());
                        change_control = true;
                        break;
                    case 's':
                        z80_update(cpu_hw, bus_data_read(), bus_get_control_in());
                        z80_update(cpu_emu, bus_data_read(), bus_get_control_in());
                        bus_update(cpu_hw);
                        if(!compare_pin_state(cpu_hw, cpu_emu)){
                            printf("------------------------\n");
                            printf("Pin state missmatch\n");
                            printf("Emulator op: %s\n", cpu_emu->internal.state.opcode->name);
                            PRINT_STATE(cpu_emu);
                            printf("Hardware: \n");
                            PRINT_STATE(cpu_hw);
                            printf("Press q to quit\n");
                            printf("------------------------\n");
                        }
                        if(bus_get_system_control_rd() == LOW){
                            memory_read();
                        }
                        if(bus_get_system_control_wr() == LOW){
                            memory_write();
                        }
                        break;
                    case 'f':
                        printf("Z: %s, S: %s, Y: %s, H: %s, X: %s, P/V: %s, N: %s, C: %s\n",
                                cpu_emu->internal.registers.F & FLAG_Z_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_S_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_Y_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_H_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_X_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_P_V_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_N_B ? "1" : "0",
                                cpu_emu->internal.registers.F & FLAG_C_B ? "1" : "0");
                        break;
                    default:
                        break;
                }
                if(!change_control)
                    PRINT_STATE(cpu_hw);
                else
                    PRINT_STATE_NO_CLOCK(cpu_hw);
                change_control = false;
            }
            termios.c_lflag |= (ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
        }
        else{
            bus_set_cpu_control_reset(LOW);
            for(uint8_t i=0; i < 16; i++){
                z80_update(cpu_hw, bus_data_read(), bus_get_control_in());
                z80_update(cpu_emu, bus_data_read(), bus_get_control_in());
            }   
            bus_set_cpu_control_reset(HIGH);

            int i = 0;
            while(bus_get_cpu_control_halt()==HIGH){
                z80_update(cpu_hw, bus_data_read(), bus_get_control_in());
                z80_update(cpu_emu, bus_data_read(), bus_get_control_in());
                bus_update(cpu_hw);
                if(!compare_pin_state(cpu_hw, cpu_emu)){
                    printf("Pin state missmatch\n");
                    printf("Emulator op: %s\n", cpu_emu->internal.state.opcode->name);
                    PRINT_STATE(cpu_emu);
                    printf("Hardware:\n");
                    PRINT_STATE(cpu_hw);
                }
                if(bus_get_system_control_rd() == LOW){
                    memory_read();
                }
                if(bus_get_system_control_wr() == LOW){
                    memory_write();
                }
                i++;
                if(i > 2000){
                    PRINT_STATE(cpu_hw);
                    printf("Press 'q' to quit 'c' to continue\n");
                    char c = getchar();
                    while(c != 'q' && c != 'c'){
                        c = getchar();
                    }
                    if(c == 'q'){
                        break;
                    }
                    i = 0;
                }
            }
        }
        printf("Clocks: %lu\n", z80_get_clock(cpu_hw));
        memory_dump("memdump.bin");

        putchar('\n');
        printf("System end\n");

        return 0;
    }
    
    z80_t* cpu = NULL;
    if(hardware){
        cpu = z80_init(Z80_HARDWARE);
    }else{
        cpu = z80_init(Z80_EMULATOR);
    }


    if(step_mode){
            struct termios termios;
            tcgetattr(STDIN_FILENO, &termios);
            termios.c_lflag &=  ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
            z80_clock_t clock = z80_get_clock(cpu);

            printf("Step mode\n");
            printf("Press h for help\n");
            PRINT_STATE_SIGNATURE();
            PRINT_STATE_NO_CLOCK(cpu);
            char c;
            bool change_control = false;
            while((c = getchar()) != 'q'){
                clock = z80_get_clock(cpu);
                switch(c){
                    case 'h':
                        printf("\n\n\nh: help\n");
                        printf("w: toggle WAIT\n");
                        printf("i: toggle INT\n");
                        printf("n: toggle NMI\n");
                        printf("r: toggle RESET\n");
                        printf("b: toggle BUSRQ\n");
                        printf("s: half_step\n");
                        printf("f: emulator flags");
                        printf("q: quit\n");
                        printf("Clock\tM1\tMREQ\tIORQ\tRD\tWR\tRFSH\tHALT\tBUSACK\tWAIT\tINT\tNMI\tBUSRQ\tRESET\tADDR\tDATA\tBUS_DATA\n");
                        break;
                    case 'w':
                        bus_set_cpu_control_wait(!bus_get_cpu_control_wait());
                        change_control = true;
                        break;
                    case 'i':
                        bus_set_cpu_control_interupt(!bus_get_cpu_control_interupt());
                        change_control = true;
                        break;
                    case 'n':
                        bus_set_cpu_control_nmi(!bus_get_cpu_control_nmi());
                        change_control = true;
                        break;
                    case 'r':
                        bus_set_cpu_control_reset(!bus_get_cpu_control_reset());
                        change_control = true;
                        break;
                    case 'b':
                        bus_set_cpu_bus_control_busrq(!bus_get_cpu_bus_control_busrq());
                        change_control = true;
                        break;
                    case 's':
                        z80_update(cpu, bus_data_read(), bus_get_control_in());
                        bus_update(cpu);
                        if(bus_get_system_control_rd() == LOW){
                            memory_read();
                        }
                        if(bus_get_system_control_wr() == LOW){
                            memory_write();
                        }
                        break;
                    case 'f':
                        if(cpu->identity == Z80_EMULATOR){
                            printf("Z: %s, S: %s, Y: %s, H: %s, X: %s, P/V: %s, N: %s, C: %s\n",
                                cpu->internal.registers.F & FLAG_Z_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_S_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_Y_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_H_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_X_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_P_V_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_N_B ? "1" : "0",
                                cpu->internal.registers.F & FLAG_C_B ? "1" : "0");
                        }
                        break;
                    default:
                        break;
                }
                if(!change_control)
                    PRINT_STATE(cpu);
                else
                    PRINT_STATE_NO_CLOCK(cpu);
                change_control = false;
            }
            termios.c_lflag |= (ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
       }
    else{
        bus_set_cpu_control_reset(LOW);
        for(uint8_t i=0; i < 16; i++){
            z80_update(cpu, bus_data_read(), bus_get_control_in());
        }   
        bus_set_cpu_control_reset(HIGH);

        int i = 0;
        while(bus_get_cpu_control_halt()==HIGH){
            z80_update(cpu, bus_data_read(), bus_get_control_in());
            bus_update(cpu);
            if(bus_get_system_control_rd() == LOW){
                memory_read();
            }
            if(bus_get_system_control_wr() == LOW){
                memory_write();
            }
            i++;
            if(i > 2000){
                PRINT_STATE(cpu);
                printf("Press 'q' to quit 'c' to continue\n");
                char c = getchar();
                while(c != 'q' && c != 'c'){
                    c = getchar();
                }
                if(c == 'q'){
                    break;
                }
                i = 0;
            }
        }
    }

    printf("Clocks: %lu\n", z80_get_clock(cpu));
    memory_dump("memdump.bin");

    putchar('\n');
    printf("System end\n");
    return 0;
}

#endif
