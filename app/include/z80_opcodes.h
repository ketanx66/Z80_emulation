#ifndef Z80_OPCODES_H
#define Z80_OPCODES_H

#include <types.h>
#include <constants.h>
#include <micro_code.h>

typedef enum state_enum{
    STANDARD_FETCH,
    FETCH,
    FETCH_ED,
    FETCH_CB,
    FETCH_DD,
    FETCH_FD,
    FETCH_FDCB,
    FETCH_DDCB,
    OPCODE,
    WAIT,
    HALT,
    NMI,
    BUSREQ,
    IMPENDING_RESET,
    RESET
}state_enum;

typedef enum step_enum{
    STEP_ADVANCE,
    STEP_ZERO,
    STEP_WAIT,
    STEP_ID
} step_enum;

typedef enum interrupt_mode_t{
    IM0,
    IM1,
    IM2
}interrupt_mode_t;

typedef struct T_half_cycle{
    uint16_t no_micro_code;
    micro_code_t* micro_code;
} T_half_cycle;

typedef struct opcode{
    uint16_t no_T_half_cycles;
    T_half_cycle* T_half_cycles;
    T_half_cycle* next_fetch;
    char* name;
} opcode;

typedef struct states{
    state_enum STATE;
    state_enum SAVE_STATE;
    interrupt_mode_t interrupt_mode;
    opcode* opcode;
    uint32_t step;
    step_enum step_op;
    step_enum save_step_op;
    uint8_t reset_routine_counter;
    uint8_t pending_nmi;
    uint8_t pending_busreq;
    uint8_t pending_counter;
    T_half_cycle standard_fetch[6];
    T_half_cycle reset[4];
    T_half_cycle halt[8];
    T_half_cycle nmi[22];
}states;

void init_opcodes();

#endif //Z80_OPCODES_H
