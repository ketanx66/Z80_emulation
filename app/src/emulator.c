#include <micro_code.h>
#include <z80_priv.h>
#include <z80.h>
#include <types.h>
#include <z80_opcodes.h>
#include <emulator.h>
#include <opcodes.h>
#include <string.h>

void advance_step(z80_t* cpu){
    switch (cpu->internal.state.step_op)
    {
    case STEP_ADVANCE:
        cpu->internal.state.step++;
        break;
    case STEP_ZERO:
        cpu->internal.state.step = 0;
        cpu->internal.state.step_op = STEP_ADVANCE;
        break;
    case STEP_WAIT:
        cpu->internal.state.step -= 2;
        cpu->internal.state.step_op = STEP_ADVANCE;
        break;
    case STEP_ID:
    default:
        break;
    }
}

void check_for_reset(z80_t* cpu){
    execute_micro_code(cpu, IN_CONTROL);
    if(cpu->internal.control_in.reset == LOW){
        cpu->internal.state.reset_routine_counter++;
        if(cpu->internal.state.reset_routine_counter == 3){
            cpu->internal.registers.PC = 0xffff;
            execute_micro_code(cpu,SET_ADDRESS_BUS_FROM_PC);
            execute_micro_code(cpu,OUT_ADDRESS_BUS);
            execute_micro_code(cpu,RESET_CPU);
            execute_micro_code(cpu,OUT_CONTROL);
            cpu->internal.state.STATE = IMPENDING_RESET;
        }
    } else {
        if(cpu->internal.state.reset_routine_counter >= 3){
            cpu->internal.state.STATE = RESET;
            cpu->internal.state.step_op = STEP_ZERO;
        }
        cpu->internal.state.reset_routine_counter = 0;
    }
}

void handle_busreq(z80_t* cpu){
    if(cpu->internal.state.pending_busreq == 1){
        cpu->internal.state.pending_counter--;
        if(cpu->internal.state.pending_counter == 0){
            cpu->internal.state.SAVE_STATE = cpu->internal.state.STATE;
            cpu->internal.state.save_step_op = cpu->internal.state.step_op;
            cpu->internal.state.STATE = BUSREQ;
            cpu->internal.state.pending_busreq = 0;
            cpu->internal.state.step_op = STEP_ID;
        }
    }
    if(cpu->internal.state.pending_busreq == 2){
        cpu->internal.state.pending_counter--;
        if(cpu->internal.state.pending_counter == 0){
            cpu->internal.state.STATE = cpu->internal.state.SAVE_STATE;
            cpu->internal.state.pending_busreq = 0;
            cpu->internal.state.step_op = cpu->internal.state.save_step_op;
            execute_micro_code(cpu, SET_BUSACK_TO_HIGH);
            execute_micro_code(cpu, OUT_CONTROL);
        }
    }
}

void check_for_nmi(z80_t* cpu){
    if(cpu->internal.state.STATE != NMI){
        if(cpu->internal.control_in.nmi == LOW){
            cpu->internal.state.pending_nmi = 1;
        }
    }
}


void execute_T(z80_t* cpu, T_half_cycle* T){
    execute_micro_code(cpu, IN_CONTROL);
    for(int i=0; i < T->no_micro_code; i++){
        execute_micro_code(cpu, T->micro_code[i]);
    }
    execute_micro_code(cpu,OUT_CONTROL);
}

void emulator_tick(z80_t* cpu){
    switch (cpu->internal.state.STATE)
    {
        case STANDARD_FETCH:
        case FETCH_ED:
        case FETCH_DD:
        case FETCH_FD:
        case FETCH_CB:
        case FETCH_DDCB:
        case FETCH_FDCB:
            execute_T(cpu, &(cpu->internal.state.standard_fetch[cpu->internal.state.step]));
            break;
        case FETCH:
            execute_T(cpu, &(cpu->internal.state.opcode->next_fetch[cpu->internal.state.step]));
            break;
        case OPCODE:
            execute_T(cpu, &(cpu->internal.state.opcode->T_half_cycles[cpu->internal.state.step]));
            break;
        case HALT:
            execute_T(cpu, &(cpu->internal.state.halt[cpu->internal.state.step]));
            break;
        case RESET:
            execute_T(cpu, &(cpu->internal.state.reset[cpu->internal.state.step]));
            break;
        case NMI:
            execute_T(cpu, &(cpu->internal.state.nmi[cpu->internal.state.step]));
            break;
        case BUSREQ:
            execute_micro_code(cpu, IN_CONTROL);
            execute_micro_code(cpu, SET_MREQ_TO_HIGH);
            execute_micro_code(cpu, SET_RD_TO_HIGH);
            execute_micro_code(cpu, SET_WR_TO_HIGH);
            execute_micro_code(cpu, SET_RFSH_TO_HIGH);
            execute_micro_code(cpu, SET_BUSACK_TO_LOW);
            execute_micro_code(cpu, OUT_CONTROL);
            cpu->pin_state.address = 0xffff;
            if(cpu->clock % 2 == 1 && cpu->internal.state.pending_busreq != 2){
                if(cpu->internal.control_in.busrq == HIGH){
                    cpu->internal.state.pending_busreq = 2;
                    cpu->internal.state.pending_counter = 3;
                }
            }
            break;
        default:
            break;
    }
    if(cpu->clock % 2 == 1){
        check_for_reset(cpu);
    }
    check_for_nmi(cpu);
    handle_busreq(cpu);
    advance_step(cpu);
    
}

void emulator_init(z80_t* cpu){
    cpu->internal.state.STATE = STANDARD_FETCH;
    cpu->internal.registers.AF = 0xFFFF;
    cpu->internal.registers.BC = 0xFFFF;
    cpu->internal.registers.DE = 0xFFFF;
    cpu->internal.registers.HL = 0xFFFF;
    cpu->internal.registers.WZ = 0xFFFF;
    cpu->internal.registers.IX = 0x0000;
    cpu->internal.registers.IY = 0x0000;
    cpu->internal.registers.SP = 0xFFFF;
    cpu->internal.registers.PC = 0xFFFF;
    cpu->internal.registers.IR = 0xFFFF;
    cpu->internal.registers.AF_ = 0xFFFF;
    cpu->internal.registers.BC_ = 0xFFFF;
    cpu->internal.registers.DE_ = 0xFFFF;
    cpu->internal.registers.HL_ = 0xFFFF;
    cpu->internal.registers.IFF1 = 1;
    cpu->internal.registers.IFF2 = 1;
    cpu->internal.registers.TMP = 0xFF;
    cpu->internal.registers.ACU = 0xFF;
    cpu->internal.registers.INST = 0xFF;
    cpu->internal.bus_address = 0x0000;
    cpu->internal.bus_address_ = 0x0000;
    cpu->internal.bus_data = 0x00;
    cpu->internal.control_in = (control_in_s){
        .wait = HIGH,
        .interupt = HIGH,
        .nmi = HIGH,
        .busrq = HIGH,
        .reset = HIGH
    };
        
    cpu->internal.control_out = (control_out_s){
        .m1 = HIGH,
        .mreq = HIGH,
        .rd = HIGH,
        .wr = HIGH,
        .rfsh = HIGH,
        .iorq = HIGH,
        .halt = HIGH,
        .busack = HIGH
    };

    cpu->internal.alu.saved_flags_state = 0;
    cpu->internal.alu.saved_flags_state = 0;
    cpu->internal.state.interrupt_mode = IM0;
    cpu->internal.state.opcode = &OPCODE_NOP;
    cpu->internal.state.step = 0;
    cpu->internal.state.step_op = STEP_ADVANCE;
    cpu->internal.state.reset_routine_counter = 0;
    cpu->internal.state.pending_nmi = 0;
    cpu->internal.state.pending_busreq = 0;
    cpu->internal.state.pending_counter = 0;

    {
        cpu->internal.state.standard_fetch[0].no_micro_code = 4;
        cpu->internal.state.standard_fetch[0].micro_code = malloc(sizeof(micro_code_t)*4);
        memcpy(cpu->internal.state.standard_fetch[0].micro_code,
            (micro_code_t[4]){
                SET_ADDRESS_BUS_FROM_PC,
                OUT_ADDRESS_BUS,
                SET_RFSH_TO_HIGH,
                SET_M1_TO_LOW,
            },
            sizeof(micro_code_t)*4);

        cpu->internal.state.standard_fetch[1].no_micro_code = 2;
        cpu->internal.state.standard_fetch[1].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.standard_fetch[1].micro_code,
            (micro_code_t[2]){
                SET_MREQ_TO_LOW,
                SET_RD_TO_LOW,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.standard_fetch[2].no_micro_code = 1;
        cpu->internal.state.standard_fetch[2].micro_code = malloc(sizeof(micro_code_t)*1);
        memcpy(cpu->internal.state.standard_fetch[2].micro_code,
            (micro_code_t[1]){
                INC_ADDRESS_BUS
            },
            sizeof(micro_code_t)*1);

        cpu->internal.state.standard_fetch[3].no_micro_code = 2;
        cpu->internal.state.standard_fetch[3].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.standard_fetch[3].micro_code,
            (micro_code_t[2]){
                CHECK_FOR_WAIT,
                SET_REG_PC_FROM_ADDRESS_BUS_,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.standard_fetch[4].no_micro_code = 9;
        cpu->internal.state.standard_fetch[4].micro_code = malloc(sizeof(micro_code_t)*9);
        memcpy(cpu->internal.state.standard_fetch[4].micro_code,
            (micro_code_t[9]){
                IN_DATA,
                SET_M1_TO_HIGH,
                SET_MREQ_TO_HIGH,
                SET_RD_TO_HIGH,
                SET_RFSH_TO_LOW,
                SET_REG_TMP,
                SET_REG_INST,
                SET_ADDRESS_BUS_FROM_IR,
                
                OUT_ADDRESS_BUS
            },
            sizeof(micro_code_t)*9);

        cpu->internal.state.standard_fetch[5].no_micro_code = 4;
        cpu->internal.state.standard_fetch[5].micro_code = malloc(sizeof(micro_code_t)*4);
        memcpy(cpu->internal.state.standard_fetch[5].micro_code,
            (micro_code_t[4]){
                SET_MREQ_TO_LOW,
                INC_REG_R,
                CHECK_FOR_BUSREQ,
                CHANGE_STATE_TO_OPCODE
            },
            sizeof(micro_code_t)*4);
    }//STANDARD_FETCH

    {
        cpu->internal.state.halt[0].no_micro_code = 5;
        cpu->internal.state.halt[0].micro_code = malloc(sizeof(micro_code_t)*5);
        memcpy(cpu->internal.state.halt[0].micro_code,
            (micro_code_t[5]){
                SET_ADDRESS_BUS_FROM_PC,
                OUT_ADDRESS_BUS,
                SET_RFSH_TO_HIGH,
                SET_M1_TO_LOW,
                SET_HALT_TO_LOW,
            },
            sizeof(micro_code_t)*5);

        cpu->internal.state.halt[1].no_micro_code = 2;
        cpu->internal.state.halt[1].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.halt[1].micro_code,
            (micro_code_t[2]){
                SET_MREQ_TO_LOW,
                SET_RD_TO_LOW,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.halt[2].no_micro_code = 0;
        cpu->internal.state.halt[2].micro_code = NULL;

        cpu->internal.state.halt[3].no_micro_code = 1;
        cpu->internal.state.halt[3].micro_code = malloc(sizeof(micro_code_t)*1);
        memcpy(cpu->internal.state.halt[3].micro_code,
            (micro_code_t[1]){
                CHECK_FOR_WAIT,
            },
            sizeof(micro_code_t)*1);
        
        cpu->internal.state.halt[4].no_micro_code = 6;
        cpu->internal.state.halt[4].micro_code = malloc(sizeof(micro_code_t)*6);
        memcpy(cpu->internal.state.halt[4].micro_code,
            (micro_code_t[6]){
                SET_M1_TO_HIGH,
                SET_MREQ_TO_HIGH,
                SET_RD_TO_HIGH,
                SET_RFSH_TO_LOW,
                SET_ADDRESS_BUS_FROM_IR,
                OUT_ADDRESS_BUS
            },
            sizeof(micro_code_t)*6);

        cpu->internal.state.halt[5].no_micro_code = 3;
        cpu->internal.state.halt[5].micro_code = malloc(sizeof(micro_code_t)*3);
        memcpy(cpu->internal.state.halt[5].micro_code,
            (micro_code_t[3]){
                SET_MREQ_TO_LOW,
                INC_REG_R,
                CHECK_FOR_BUSREQ,
            },
            sizeof(micro_code_t)*3);

        cpu->internal.state.halt[6].no_micro_code = 0;
        cpu->internal.state.halt[6].micro_code = NULL;

        cpu->internal.state.halt[7].no_micro_code = 2;
        cpu->internal.state.halt[7].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.halt[7].micro_code,
            (micro_code_t[2]){
                SET_MREQ_TO_HIGH,
                CHANGE_STATE_TO_HALT
            },
            sizeof(micro_code_t)*2);
    }//HALT

    {
        cpu->internal.state.reset[0].no_micro_code = 0;
        cpu->internal.state.reset[0].micro_code = NULL;

        cpu->internal.state.reset[1].no_micro_code = 0;
        cpu->internal.state.reset[1].micro_code = NULL;

        cpu->internal.state.reset[2].no_micro_code = 0;
        cpu->internal.state.reset[2].micro_code = NULL;

        cpu->internal.state.reset[3].no_micro_code = 1;
        cpu->internal.state.reset[3].micro_code = malloc(sizeof(micro_code_t)*1);
        memcpy(cpu->internal.state.reset[3].micro_code,
            (micro_code_t[1]){
                CHANGE_STATE_TO_STANDARD_FETCH
            },
            sizeof(micro_code_t)*1);
    }//RESET

    {
        cpu->internal.state.nmi[0].no_micro_code = 4;
        cpu->internal.state.nmi[0].micro_code = malloc(sizeof(micro_code_t)*4);
        memcpy(cpu->internal.state.nmi[0].micro_code,
            (micro_code_t[4]){
                SET_ADDRESS_BUS_FROM_PC,
                OUT_ADDRESS_BUS,
                SET_RFSH_TO_HIGH,
                SET_M1_TO_LOW,
            },
            sizeof(micro_code_t)*4);

        cpu->internal.state.nmi[1].no_micro_code = 2;
        cpu->internal.state.nmi[1].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[1].micro_code,
            (micro_code_t[2]){
                SET_MREQ_TO_LOW,
                SET_RD_TO_LOW,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[2].no_micro_code = 1;
        cpu->internal.state.nmi[2].micro_code = malloc(sizeof(micro_code_t)*1);
        memcpy(cpu->internal.state.nmi[2].micro_code,
            (micro_code_t[1]){
                INC_ADDRESS_BUS
            },
            sizeof(micro_code_t)*1);

        cpu->internal.state.nmi[3].no_micro_code = 2;
        cpu->internal.state.nmi[3].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[3].micro_code,
            (micro_code_t[2]){
                CHECK_FOR_WAIT,
                SKIP_INSTRUCTION,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[4].no_micro_code = 9;
        cpu->internal.state.nmi[4].micro_code = malloc(sizeof(micro_code_t)*9);
        memcpy(cpu->internal.state.nmi[4].micro_code,
            (micro_code_t[9]){
                IN_DATA,
                SET_M1_TO_HIGH,
                SET_MREQ_TO_HIGH,
                SET_RD_TO_HIGH,
                SET_RFSH_TO_LOW,
                SET_REG_TMP,
                SET_REG_INST,
                SET_ADDRESS_BUS_FROM_IR,
                OUT_ADDRESS_BUS
            },
            sizeof(micro_code_t)*9);

        cpu->internal.state.nmi[5].no_micro_code = 2;
        cpu->internal.state.nmi[5].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[5].micro_code,
            (micro_code_t[2]){
                SET_MREQ_TO_LOW,
                INC_REG_R,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[6].no_micro_code = 3;
        cpu->internal.state.nmi[6].micro_code = malloc(sizeof(micro_code_t)*3);
        memcpy(cpu->internal.state.nmi[6].micro_code,
            (micro_code_t[3]){
                SET_ADDRESS_BUS_FROM_SP,
                SET_DATA_BUS_66h,
                SET_REG_TMP,
            },
            sizeof(micro_code_t)*3);

        cpu->internal.state.nmi[7].no_micro_code = 3;
        cpu->internal.state.nmi[7].micro_code = malloc(sizeof(micro_code_t)*3);
        memcpy(cpu->internal.state.nmi[7].micro_code,
            (micro_code_t[3]){
                DEC_ADDRESS_BUS,
                SET_MREQ_TO_HIGH,
                SET_REG_Z_FROM_TMP,
            },
            sizeof(micro_code_t)*3);

        cpu->internal.state.nmi[8].no_micro_code = 2;
        cpu->internal.state.nmi[8].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[8].micro_code,
            (micro_code_t[2]){
                SET_RFSH_TO_HIGH,
                SET_REG_SP_FROM_ADDRESS_BUS_,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[9].no_micro_code = 2;
        cpu->internal.state.nmi[9].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[9].micro_code,
            (micro_code_t[2]){
                SET_REG_TMP_FROM_PCH,
                SET_ADDRESS_BUS_FROM_SP,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[10].no_micro_code = 2;
        cpu->internal.state.nmi[10].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[10].micro_code,
            (micro_code_t[2]){
                OUT_ADDRESS_BUS,
                SET_DATA_BUS_FROM_TMP,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[11].no_micro_code = 3;
        cpu->internal.state.nmi[11].micro_code = malloc(sizeof(micro_code_t)*3);
        memcpy(cpu->internal.state.nmi[11].micro_code,
            (micro_code_t[3]){
                DEC_ADDRESS_BUS,
                SET_MREQ_TO_LOW,
                OUT_DATA,
            },
            sizeof(micro_code_t)*3);

        cpu->internal.state.nmi[12].no_micro_code = 2;
        cpu->internal.state.nmi[12].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[12].micro_code,
            (micro_code_t[2]){
                OUT_DATA,
			    SET_REG_SP_FROM_ADDRESS_BUS_,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[13].no_micro_code = 3;
        cpu->internal.state.nmi[13].micro_code = malloc(sizeof(micro_code_t)*3);
        memcpy(cpu->internal.state.nmi[13].micro_code,
            (micro_code_t[3]){
                OUT_DATA,
                CHECK_FOR_WAIT,
                SET_WR_TO_LOW,
            },
            sizeof(micro_code_t)*3);

        cpu->internal.state.nmi[14].no_micro_code = 1;
        cpu->internal.state.nmi[14].micro_code = malloc(sizeof(micro_code_t)*1);
        memcpy(cpu->internal.state.nmi[14].micro_code,
            (micro_code_t[1]){
                OUT_DATA,
            },
            sizeof(micro_code_t)*1);

        cpu->internal.state.nmi[15].no_micro_code = 5;
        cpu->internal.state.nmi[15].micro_code = malloc(sizeof(micro_code_t)*5);
        memcpy(cpu->internal.state.nmi[15].micro_code,
            (micro_code_t[5]){
                OUT_DATA,
                SET_WR_TO_HIGH,
                SET_MREQ_TO_HIGH,
                SET_ADDRESS_BUS_FROM_SP,
                SET_REG_TMP_FROM_PCL,
            },
            sizeof(micro_code_t)*5);

        cpu->internal.state.nmi[16].no_micro_code = 2;
        cpu->internal.state.nmi[16].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[16].micro_code,
            (micro_code_t[2]){
                OUT_ADDRESS_BUS,
                SET_DATA_BUS_FROM_TMP,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[17].no_micro_code = 2;
        cpu->internal.state.nmi[17].micro_code = malloc(sizeof(micro_code_t)*2);
        memcpy(cpu->internal.state.nmi[17].micro_code,
            (micro_code_t[2]){
                SET_MREQ_TO_LOW,
			    OUT_DATA,
            },
            sizeof(micro_code_t)*2);

        cpu->internal.state.nmi[18].no_micro_code = 3;
        cpu->internal.state.nmi[18].micro_code = malloc(sizeof(micro_code_t)*3);
        memcpy(cpu->internal.state.nmi[18].micro_code,
            (micro_code_t[3]){
                OUT_DATA,
			    SET_DATA_BUS_00h,
			    SET_REG_TMP,
            },
            sizeof(micro_code_t)*3);

        cpu->internal.state.nmi[19].no_micro_code = 6;
        cpu->internal.state.nmi[19].micro_code = malloc(sizeof(micro_code_t)*6);
        memcpy(cpu->internal.state.nmi[19].micro_code,
            (micro_code_t[6]){
                SET_REG_W_FROM_TMP,
                SET_REG_TMP_FROM_PCL,
                SET_DATA_BUS_FROM_TMP,
                OUT_DATA,
                CHECK_FOR_WAIT,
                SET_WR_TO_LOW,
            },
            sizeof(micro_code_t)*6);

        cpu->internal.state.nmi[20].no_micro_code = 1;
        cpu->internal.state.nmi[20].micro_code = malloc(sizeof(micro_code_t)*1);
        memcpy(cpu->internal.state.nmi[20].micro_code,
            (micro_code_t[1]){
                OUT_DATA,
            },
            sizeof(micro_code_t)*1);

        cpu->internal.state.nmi[21].no_micro_code = 8;
        cpu->internal.state.nmi[21].micro_code = malloc(sizeof(micro_code_t)*8);
        memcpy(cpu->internal.state.nmi[21].micro_code,
            (micro_code_t[8]){
                OUT_DATA,
                SET_WR_TO_HIGH,
                SET_MREQ_TO_HIGH,
                SET_ADDRESS_BUS_FROM_WZ,
                ID_ADDRESS_BUS,
                SET_REG_PC_FROM_ADDRESS_BUS_,
                SET_ADDRESS_BUS_FROM_PC,
                CHANGE_STATE_TO_STANDARD_FETCH,
            },
            sizeof(micro_code_t)*8);
    }//NMI

    init_opcodes();
}