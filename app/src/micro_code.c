#include <z80_priv.h>
#include <z80.h>
#include <opcodes.h>
#include <micro_code.h>
#include <types.h>
#include <bus.h>

void execute_micro_code(z80_t *z80, micro_code_t micro_code){
    switch (micro_code)
    {
        //BUS CONTROL
        case IN_DATA:
            z80->internal.bus_data = z80->pin_state.data;
            break;
        case OUT_DATA:
            z80->pin_state.data = z80->internal.bus_data;
            break;
        case OUT_CONTROL:
            z80->pin_state.control_out = z80->internal.control_out;
            break;
        case IN_CONTROL:
            z80->internal.control_in = z80->pin_state.control_in;
            break;
        case SET_DATA_BUS_FROM_A:
            z80->internal.bus_data = z80->internal.registers.A;
            break;
        case SET_DATA_BUS_FROM_F:
            z80->internal.bus_data = z80->internal.registers.F;
            break;
        case SET_DATA_BUS_FROM_W:
            z80->internal.bus_data = z80->internal.registers.W;
            break;
        case SET_DATA_BUS_FROM_Z:
            z80->internal.bus_data = z80->internal.registers.Z;
            break;
        case SET_DATA_BUS_FROM_B:
            z80->internal.bus_data = z80->internal.registers.B;
            break;
        case SET_DATA_BUS_FROM_C:
            z80->internal.bus_data = z80->internal.registers.C;
            break;
        case SET_DATA_BUS_FROM_D:
            z80->internal.bus_data = z80->internal.registers.D;
            break;
        case SET_DATA_BUS_FROM_E:
            z80->internal.bus_data = z80->internal.registers.E;
            break;
        case SET_DATA_BUS_FROM_H:
            z80->internal.bus_data = z80->internal.registers.H;
            break;
        case SET_DATA_BUS_FROM_L:
            z80->internal.bus_data = z80->internal.registers.L;
            break;
        case SET_DATA_BUS_FROM_I:
            z80->internal.bus_data = z80->internal.registers.I;
            break;
        case SET_DATA_BUS_FROM_R:
            z80->internal.bus_data = z80->internal.registers.R;
            break;
        case SET_DATA_BUS_FROM_TMP:
            z80->internal.bus_data = z80->internal.registers.TMP;
            break;
        case SET_DATA_BUS_FROM_PCH:
            z80->internal.bus_data = z80->internal.registers.PCH;
            break;
        case SET_DATA_BUS_FROM_PCL:
            z80->internal.bus_data = z80->internal.registers.PCL;
            break;
        case SET_DATA_BUS_FROM_SPH:
            z80->internal.bus_data = z80->internal.registers.SPH;
            break;
        case SET_DATA_BUS_FROM_SPL:
            z80->internal.bus_data = z80->internal.registers.SPL;
            break;
        case SET_DATA_BUS_FROM_IXH:
            z80->internal.bus_data = z80->internal.registers.IXH;
            break;
        case SET_DATA_BUS_FROM_IXL:
            z80->internal.bus_data = z80->internal.registers.IXL;
            break;
        case SET_DATA_BUS_FROM_IYH: 
            z80->internal.bus_data = z80->internal.registers.IYH;
            break;
        case SET_DATA_BUS_FROM_IYL:
            z80->internal.bus_data = z80->internal.registers.IYL;
            break;
        case SET_DATA_BUS_00h:
            z80->internal.bus_data = 0x00;
            break;
        case SET_DATA_BUS_08h:
            z80->internal.bus_data = 0x08;
            break;
        case SET_DATA_BUS_10h:
            z80->internal.bus_data = 0x10;
            break;
        case SET_DATA_BUS_18h:
            z80->internal.bus_data = 0x18;
            break;
        case SET_DATA_BUS_20h:
            z80->internal.bus_data = 0x20;
            break;
        case SET_DATA_BUS_28h:
            z80->internal.bus_data = 0x28;
            break;
        case SET_DATA_BUS_30h:
            z80->internal.bus_data = 0x30;
            break;
        case SET_DATA_BUS_38h:
            z80->internal.bus_data = 0x38;
            break;
        case SET_DATA_BUS_40h:
            z80->internal.bus_data = 0x40;
            break;
        case SET_DATA_BUS_66h:
            z80->internal.bus_data = 0x66;
            break;

        //CONTROL
        case SET_M1_TO_LOW:
            z80->internal.control_out.m1 = LOW;
            break;
        case SET_M1_TO_HIGH:
            z80->internal.control_out.m1 = HIGH;
            break;
        case SET_MREQ_TO_LOW:
            z80->internal.control_out.mreq = LOW;
            break;
        case SET_MREQ_TO_HIGH:
            z80->internal.control_out.mreq = HIGH;
            break;
        case SET_IORQ_TO_LOW:
            z80->internal.control_out.iorq = LOW;
            break;                  
        case SET_IORQ_TO_HIGH:
            z80->internal.control_out.iorq = HIGH;
            break;
        case SET_RD_TO_LOW:
            z80->internal.control_out.rd = LOW;
            break;
        case SET_RD_TO_HIGH:    
            z80->internal.control_out.rd = HIGH;
            break;
        case SET_WR_TO_LOW:
            z80->internal.control_out.wr = LOW;
            break;
        case SET_WR_TO_HIGH:
            z80->internal.control_out.wr = HIGH;
            break;
        case SET_RFSH_TO_LOW:
            z80->internal.control_out.rfsh = LOW;
            break;
        case SET_RFSH_TO_HIGH:
            z80->internal.control_out.rfsh = HIGH;
            break;
        case SET_HALT_TO_LOW:
            z80->internal.control_out.halt = LOW;
            break;
        case SET_HALT_TO_HIGH:  
            z80->internal.control_out.halt = HIGH;
            break;
        case SET_BUSACK_TO_LOW:
            z80->internal.control_out.busack = LOW;
            break;
        case SET_BUSACK_TO_HIGH:
            z80->internal.control_out.busack = HIGH;
            break;

        //ALU
        case ALU_INC_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = z80->internal.registers.F & FLAG_C_B;
                if(z80->internal.registers.TMP & 0x7f){
                    new_flags |= FLAG_P_V_B;  
                }
                if(z80->internal.registers.TMP & 0x07){
                    new_flags |= FLAG_H_B;  
                }
                z80->internal.registers.A = z80->internal.registers.TMP + 1;
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_DEC_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = z80->internal.registers.F & FLAG_C_B;
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_P_V_B;  
                }
                if(z80->internal.registers.TMP & 0xf8){
                    new_flags |= FLAG_H_B;  
                }
                z80->internal.registers.A = z80->internal.registers.TMP - 1;
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_RLC_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = z80->internal.registers.TMP << 1 |
                                            z80->internal.registers.TMP >> 7;
                
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(!(z80->internal.registers.A & 0x01)){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_RRC_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = (z80->internal.registers.F & FLAG_S_B) |
                            (z80->internal.registers.F & FLAG_Z_B) |
                            (z80->internal.registers.F & FLAG_P_V_B);
                if(z80->internal.registers.TMP & 0x01){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = z80->internal.registers.TMP >> 1 |
                                            z80->internal.registers.TMP << 7;
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_RL_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = (z80->internal.registers.F & FLAG_S_B) |
                            (z80->internal.registers.F & FLAG_Z_B) |
                            (z80->internal.registers.F & FLAG_P_V_B);
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = (z80->internal.registers.TMP << 1);
                if(z80->internal.registers.F & FLAG_C_B){
                    z80->internal.registers.A |= 0x01;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_RR_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = (z80->internal.registers.F & FLAG_S_B) |
                            (z80->internal.registers.F & FLAG_Z_B) |
                            (z80->internal.registers.F & FLAG_P_V_B);
                if(z80->internal.registers.TMP & 0x01){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = (z80->internal.registers.TMP >> 1);
                if(z80->internal.registers.F & FLAG_C_B){
                    z80->internal.registers.A |= 0x80;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SLA_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = (z80->internal.registers.F & FLAG_S_B) |
                            (z80->internal.registers.F & FLAG_Z_B) |
                            (z80->internal.registers.F & FLAG_P_V_B);
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = (z80->internal.registers.TMP << 1);
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SRA_TMP:
            {
                uint8_t new_flags = 0;
                new_flags = (z80->internal.registers.F & FLAG_S_B) |
                            (z80->internal.registers.F & FLAG_Z_B) |
                            (z80->internal.registers.F & FLAG_P_V_B);
                if(z80->internal.registers.TMP & 0x01){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = (z80->internal.registers.TMP >> 1) |
                                            (z80->internal.registers.TMP & 0x80);
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SRL_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x01){
                    new_flags |= FLAG_C_B;  
                }
                z80->internal.registers.A = (z80->internal.registers.TMP >> 1);
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(!(z80->internal.registers.A % 2)){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_ADD:
            {
                uint8_t new_flags = 0;
                uint8_t carry = 0;
                uint8_t half_carry = 0;
                uint8_t overflow = 0;
                z80->internal.registers.A = z80->internal.registers.ACU + z80->internal.registers.TMP;
                carry = z80->internal.registers.A < z80->internal.registers.ACU;
                half_carry = (z80->internal.registers.A & 0x0f) < (z80->internal.registers.ACU & 0x0f);
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(half_carry){
                    new_flags |= FLAG_H_B;
                }
                if(carry){
                    new_flags |= FLAG_C_B;
                    new_flags |= FLAG_P_V_B; //TODO CHECK OVERFLOW
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_ADC:
            {
                uint8_t new_flags = 0;
                uint8_t carry = 0;
                uint8_t half_carry = 0;
                uint8_t overflow = 0;
                z80->internal.registers.A = z80->internal.registers.ACU + z80->internal.registers.TMP;
                if(z80->internal.registers.F & FLAG_C_B){
                    z80->internal.registers.A++;
                }
                carry = z80->internal.registers.A < z80->internal.registers.ACU;
                half_carry = (z80->internal.registers.A & 0x0f) < (z80->internal.registers.ACU & 0x0f);
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(half_carry){
                    new_flags |= FLAG_H_B;
                }
                if(carry){
                    new_flags |= FLAG_C_B;
                    new_flags |= FLAG_P_V_B; //TODO CHECK OVERFLOW
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SUB:
            {
                uint8_t new_flags = 0;
                uint8_t carry = 0;
                uint8_t half_carry = 0;
                uint8_t overflow = 0;
                z80->internal.registers.A = z80->internal.registers.ACU - z80->internal.registers.TMP;
                carry = z80->internal.registers.A > z80->internal.registers.ACU;
                half_carry = (z80->internal.registers.A & 0x0f) > (z80->internal.registers.ACU & 0x0f);
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(half_carry){
                    new_flags |= FLAG_H_B;
                }
                if(carry){
                    new_flags |= FLAG_C_B;
                    new_flags |= FLAG_P_V_B; //TODO CHECK OVERFLOW
                }
                new_flags |= FLAG_N_B;
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SBC:
            {
                uint8_t new_flags = 0;
                uint8_t carry = 0;
                uint8_t half_carry = 0;
                uint8_t overflow = 0;
                z80->internal.registers.A = z80->internal.registers.ACU - z80->internal.registers.TMP;
                if(z80->internal.registers.F & FLAG_C_B){
                    z80->internal.registers.A--;
                }
                carry = z80->internal.registers.A > z80->internal.registers.ACU;
                half_carry = (z80->internal.registers.A & 0x0f) > (z80->internal.registers.ACU & 0x0f);
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(half_carry){
                    new_flags |= FLAG_H_B;
                }
                if(carry){
                    new_flags |= FLAG_C_B;
                    new_flags |= FLAG_P_V_B; //TODO CHECK OVERFLOW
                }
                new_flags |= FLAG_N_B;
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_AND:
            {
                uint8_t new_flags = 0;
                z80->internal.registers.A = z80->internal.registers.ACU & z80->internal.registers.TMP;
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.A & 0x01){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_OR:
            {
                uint8_t new_flags = 0;
                z80->internal.registers.A = z80->internal.registers.ACU | z80->internal.registers.TMP;
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(z80->internal.registers.A & 0x01){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_XOR:
            {
                uint8_t new_flags = 0;
                z80->internal.registers.A = z80->internal.registers.ACU ^ z80->internal.registers.TMP;
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(z80->internal.registers.A & 0x01){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
        case ALU_CPL:
            {
                uint8_t new_flags = 0;
                z80->internal.registers.A = ~z80->internal.registers.ACU;
                new_flags |= FLAG_N_B;
                new_flags |= FLAG_H_B;
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_CCF://TODO Check
            {
                uint8_t new_flags = 0;
                if(!(z80->internal.registers.F & FLAG_C_B)){
                    new_flags |= FLAG_C_B;
                }
                if(z80->internal.registers.F & FLAG_H_B){
                    new_flags |= FLAG_H_B;
                }
                if(z80->internal.registers.F & FLAG_S_B){
                    new_flags |= FLAG_S_B;
                }
                if(z80->internal.registers.F & FLAG_Z_B){
                    new_flags |= FLAG_Z_B;
                }
                if(z80->internal.registers.F & FLAG_P_V_B){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SCF://TODO Check
            {
                uint8_t new_flags = 0;
                new_flags |= FLAG_C_B;
                if(z80->internal.registers.F & FLAG_H_B){
                    new_flags |= FLAG_H_B;
                }
                if(z80->internal.registers.F & FLAG_S_B){
                    new_flags |= FLAG_S_B;
                }
                if(z80->internal.registers.F & FLAG_Z_B){
                    new_flags |= FLAG_Z_B;
                }
                if(z80->internal.registers.F & FLAG_P_V_B){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_CP:
            {
                uint8_t new_flags = 0;
                uint8_t carry = 0;
                uint8_t half_carry = 0;
                uint8_t tmp = z80->internal.registers.ACU - z80->internal.registers.TMP;
                if(z80->internal.registers.ACU < z80->internal.registers.TMP){
                    carry = 1;
                }
                if((z80->internal.registers.ACU & 0x0F) < (z80->internal.registers.TMP & 0x0F)){
                    half_carry = 1;
                }
                if(tmp & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(tmp == 0){
                    new_flags |= FLAG_Z_B;
                }
                new_flags |= FLAG_N_B;
                if(half_carry){
                    new_flags |= FLAG_H_B;
                }
                if(carry){
                    new_flags |= FLAG_C_B;
                }
                if((z80->internal.registers.ACU ^ z80->internal.registers.TMP) & 0x80 && (z80->internal.registers.ACU ^ tmp) & 0x80){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_NEG:
            {
                uint8_t new_flags = 0;
                uint8_t carry = 0;
                uint8_t half_carry = 0;
                uint8_t tmp = 0 - z80->internal.registers.ACU;
                z80->internal.registers.A = tmp;
                if(z80->internal.registers.ACU != 0){
                    carry = 1;
                }
                if((z80->internal.registers.ACU & 0x0F) != 0){
                    half_carry = 1;
                }
                if(tmp & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(tmp == 0){
                    new_flags |= FLAG_Z_B;
                }
                new_flags |= FLAG_N_B;
                if(half_carry){
                    new_flags |= FLAG_H_B;
                }
                if(carry){
                    new_flags |= FLAG_C_B;
                }
                if((z80->internal.registers.ACU ^ z80->internal.registers.ACU) & 0x80 && (z80->internal.registers.ACU ^ tmp) & 0x80){
                    new_flags |= FLAG_P_V_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_RLD:
            {
                uint8_t new_flags = 0;
                z80->internal.registers.A = (z80->internal.registers.ACU & 0xf0) | ((z80->internal.registers.TMP & 0xf0) >> 4);
                z80->internal.registers.TMP = ((z80->internal.registers.TMP & 0x0f)<<4) | (z80->internal.registers.ACU & 0x0f);
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(!(z80->internal.registers.A & 0x01)){
                    new_flags |= FLAG_P_V_B;
                }
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_RRD:
            {
                uint8_t new_flags = 0;
                z80->internal.registers.A = (z80->internal.registers.ACU & 0xf0) | (z80->internal.registers.TMP & 0x0f);
                z80->internal.registers.TMP = ((z80->internal.registers.TMP & 0xf0)>>4) | ((z80->internal.registers.ACU & 0x0f)<<4);
                if(z80->internal.registers.A & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.A == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(!(z80->internal.registers.A & 0x01)){
                    new_flags |= FLAG_P_V_B;
                }
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
        case ALU_TEST_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.TMP == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(z80->internal.registers.TMP % 2){
                    new_flags |= FLAG_P_V_B;
                }
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_0_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x01){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_1_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x02){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_2_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x04){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_3_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x08){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_4_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x10){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_5_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x20){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_6_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x40){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_BIT_7_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_P_V_B;
                }
                new_flags |= FLAG_H_B;
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case ALU_SET_0_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x01;
            }
            break;
        case ALU_SET_1_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x02;
            }
            break;
        case ALU_SET_2_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x04;
            }
            break;
        case ALU_SET_3_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x08;
            }
            break;
        case ALU_SET_4_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x10;
            }
            break;
        case ALU_SET_5_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x20;
            }
            break;
        case ALU_SET_6_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x40;
            }
            break;
        case ALU_SET_7_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP | 0x80;
            }
            break;
        case ALU_RES_0_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xfe;
            }
            break;
        case ALU_RES_1_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xfd;
            }
            break;
        case ALU_RES_2_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xfb;
            }
            break;
        case ALU_RES_3_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xf7;
            }
            break;
        case ALU_RES_4_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xef;
            }
            break;
        case ALU_RES_5_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xdf;
            }
            break;
        case ALU_RES_6_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0xbf;
            }
            break;
        case ALU_RES_7_TMP:
            {
                z80->internal.registers.A = z80->internal.registers.TMP & 0x7f;
            }
            break;
        case ALU_TEST_LD_IR_TMP:
            {
                uint8_t new_flags = 0;
                if(z80->internal.registers.TMP & 0x80){
                    new_flags |= FLAG_S_B;    
                }
                if(z80->internal.registers.TMP == 0){
                    new_flags |= FLAG_Z_B;
                }
                if(z80->internal.registers.IFF2 == 1){
                    new_flags |= FLAG_P_V_B;
                }
                if(z80->internal.control_in.busrq == 1 &&
                   (z80->internal.control_in.interupt == 0 || z80->internal.control_in.nmi == 0)){
                    new_flags &= ~FLAG_P_V_B;
                }
                if(z80->internal.registers.F & FLAG_C_B){
                    new_flags |= FLAG_C_B;
                }
                z80->internal.alu.flags = new_flags;
            }
            break;
        case UPDATE_FLAGS_FROM_ALU:
            {
                uint8_t flags = z80->internal.alu.saved_flags;
                flags &= ~z80->internal.alu.saved_flags_state;
                flags |= z80->internal.alu.flags;
                z80->internal.registers.F = flags;
                z80->internal.alu.saved_flags_state = 0;
                z80->internal.alu.saved_flags = 0;
            }
            break;

        //ALU FLAGS
        case SET_FLAG_C0_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b11111110;
            break;
        case SET_FLAG_C0_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b00000001;
            break;
        case SET_FLAG_N1_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b11111101;
            break;
        case SET_FLAG_N1_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b00000010;
            break;
        case SET_FLAG_P2_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b11111011;
            break;
        case SET_FLAG_P2_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b00000100;
            break;
        case SET_FLAG_X3_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b11110111;
            break;
        case SET_FLAG_X3_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b00001000;
            break;
        case SET_FLAG_H4_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b11101111;
            break;
        case SET_FLAG_H4_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b00010000;
            break;
        case SET_FLAG_Y5_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b11011111;
            break;
        case SET_FLAG_Y5_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b00100000;
            break;
        case SET_FLAG_Z6_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b10111111;
            break;
        case SET_FLAG_Z6_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b01000000;
            break;
        case SET_FLAG_S7_TO_LOW:
            z80->internal.registers.F = z80->internal.registers.F & 0b01111111;
            break;
        case SET_FLAG_S7_TO_HIGH:
            z80->internal.registers.F = z80->internal.registers.F | 0b10000000;
            break;
        case SAVE_FLAG_C0:
            z80->internal.alu.saved_flags |= FLAG_C_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_C_B;
            break;
        case SAVE_FLAG_N1:
            z80->internal.alu.saved_flags |= FLAG_N_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_N_B;
            break;
        case SAVE_FLAG_P2:
            z80->internal.alu.saved_flags |= FLAG_P_V_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_P_V_B;
            break;
        case SAVE_FLAG_X3:
            z80->internal.alu.saved_flags |= FLAG_X_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_X_B;
            break;
        case SAVE_FLAG_H4:
            z80->internal.alu.saved_flags |= FLAG_H_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_H_B;
            break;
        case SAVE_FLAG_Y5:
            z80->internal.alu.saved_flags |= FLAG_Y_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_Y_B;
            break;
        case SAVE_FLAG_Z6:
            z80->internal.alu.saved_flags |= FLAG_Z_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_Z_B;
            break;
        case SAVE_FLAG_S7:
            z80->internal.alu.saved_flags |= FLAG_S_B;
            z80->internal.alu.saved_flags_state |= z80->internal.registers.F & FLAG_S_B;
            break;
        case SET_FLAG_P_V_NON_ZERO_BC:
            {
                if(z80->internal.registers.BC != 0){
                    z80->internal.registers.F |= FLAG_P_V_B;
                } else {
                    z80->internal.registers.F &= ~FLAG_P_V_B;
                }
            }
            break;
        
        //REGISTERS
        case SET_REG_ACU:
            z80->internal.registers.ACU = z80->internal.registers.A;
            break;
        case SET_REG_TMP:
            z80->internal.registers.TMP = z80->internal.bus_data;
            break;
        case SET_REG_INST:
            z80->internal.registers.INST = z80->internal.bus_data;
            break;
        case SET_REG_A_FROM_TMP:
            z80->internal.registers.A = z80->internal.registers.TMP;
            break;
        case SET_REG_F_FROM_TMP:
            z80->internal.registers.F = z80->internal.registers.TMP;
            break;
        case SET_REG_W_FROM_TMP:
            z80->internal.registers.W = z80->internal.registers.TMP;
            break;
        case SET_REG_Z_FROM_TMP:
            z80->internal.registers.Z = z80->internal.registers.TMP;
            break;
        case SET_REG_B_FROM_TMP:
            z80->internal.registers.B = z80->internal.registers.TMP;
            break;
        case SET_REG_C_FROM_TMP:
            z80->internal.registers.C = z80->internal.registers.TMP;
            break;
        case SET_REG_D_FROM_TMP:
            z80->internal.registers.D = z80->internal.registers.TMP;
            break;
        case SET_REG_E_FROM_TMP:
            z80->internal.registers.E = z80->internal.registers.TMP;
            break;
        case SET_REG_H_FROM_TMP:
            z80->internal.registers.H = z80->internal.registers.TMP;
            break;
        case SET_REG_L_FROM_TMP:
            z80->internal.registers.L = z80->internal.registers.TMP;
            break;
        case SET_REG_I_FROM_TMP:
            z80->internal.registers.I = z80->internal.registers.TMP;
            break;
        case SET_REG_R_FROM_TMP:
            z80->internal.registers.R = z80->internal.registers.TMP;
            break;
        case SET_REG_SPL_FROM_TMP:
            z80->internal.registers.SPL = z80->internal.registers.TMP;
            break;
        case SET_REG_SPH_FROM_TMP:
            z80->internal.registers.SPH = z80->internal.registers.TMP;
            break;
        case SET_REG_PCL_FROM_TMP:
            z80->internal.registers.PCL = z80->internal.registers.TMP;
            break;
        case SET_REG_PCH_FROM_TMP:
            z80->internal.registers.PCH = z80->internal.registers.TMP;
            break;
        case SET_REG_IXL_FROM_TMP:
            z80->internal.registers.IXL = z80->internal.registers.TMP;
            break;
        case SET_REG_IXH_FROM_TMP:
            z80->internal.registers.IXH = z80->internal.registers.TMP;
            break;
        case SET_REG_IYL_FROM_TMP:
            z80->internal.registers.IYL = z80->internal.registers.TMP;
            break;
        case SET_REG_IYH_FROM_TMP:
            z80->internal.registers.IYH = z80->internal.registers.TMP;
            break;
        case SET_REG_TMP_FROM_A:
            z80->internal.registers.TMP = z80->internal.registers.A;
            break;
        case SET_REG_TMP_FROM_F:
            z80->internal.registers.TMP = z80->internal.registers.F;
            break;
        case SET_REG_TMP_FROM_W:
            z80->internal.registers.TMP = z80->internal.registers.W;
            break;
        case SET_REG_TMP_FROM_Z:
            z80->internal.registers.TMP = z80->internal.registers.Z;
            break;
        case SET_REG_TMP_FROM_B:
            z80->internal.registers.TMP = z80->internal.registers.B;
            break;
        case SET_REG_TMP_FROM_C:
            z80->internal.registers.TMP = z80->internal.registers.C;
            break;
        case SET_REG_TMP_FROM_D:
            z80->internal.registers.TMP = z80->internal.registers.D;
            break;
        case SET_REG_TMP_FROM_E:
            z80->internal.registers.TMP = z80->internal.registers.E;
            break;
        case SET_REG_TMP_FROM_H:
            z80->internal.registers.TMP = z80->internal.registers.H;
            break;
        case SET_REG_TMP_FROM_L:
            z80->internal.registers.TMP = z80->internal.registers.L;
            break;
        case SET_REG_TMP_FROM_I:
            z80->internal.registers.TMP = z80->internal.registers.I;
            break;
        case SET_REG_TMP_FROM_R:
            z80->internal.registers.TMP = z80->internal.registers.R;
            break;
        case SET_REG_TMP_FROM_SPL:
            z80->internal.registers.TMP = z80->internal.registers.SPL;
            break;
        case SET_REG_TMP_FROM_SPH:
            z80->internal.registers.TMP = z80->internal.registers.SPH;
            break;
        case SET_REG_TMP_FROM_PCL:
            z80->internal.registers.TMP = z80->internal.registers.PCL;
            break;
        case SET_REG_TMP_FROM_PCH:
            z80->internal.registers.TMP = z80->internal.registers.PCH;
            break;
        case SET_REG_TMP_FROM_IXH:
            z80->internal.registers.TMP = z80->internal.registers.IXH;
            break;
        case SET_REG_TMP_FROM_IXL:
            z80->internal.registers.TMP = z80->internal.registers.IXL;
            break;
        case SET_REG_TMP_FROM_IYH:
            z80->internal.registers.TMP = z80->internal.registers.IYH;
            break;
        case SET_REG_TMP_FROM_IYL:
            z80->internal.registers.TMP = z80->internal.registers.IYL;
            break;
        case SET_REG_IFF1_TO_HIGH:
            z80->internal.registers.IFF1 = 1;
            break;
        case SET_REG_IFF1_TO_LOW:
            z80->internal.registers.IFF1 = 0;
            break;
        case SET_REG_IFF2_TO_HIGH:
            z80->internal.registers.IFF2 = 1;
            break;
        case SET_REG_IFF2_TO_LOW:
            z80->internal.registers.IFF2 = 0;
            break;
        case MV_IFF2_TO_IFF1:
            z80->internal.registers.IFF1 = z80->internal.registers.IFF2;
            break;
        case SET_REG_PC_FROM_ADDRESS_BUS_:
            z80->internal.registers.PC = z80->internal.bus_address_;
            break;
        case SET_REG_IR_FROM_ADDRESS_BUS_:
            z80->internal.registers.IR = z80->internal.bus_address_;
            break;
        case SET_REG_HL_FROM_ADDRESS_BUS_:
            z80->internal.registers.HL = z80->internal.bus_address_;
            break;
        case SET_REG_DE_FROM_ADDRESS_BUS_:
            z80->internal.registers.DE = z80->internal.bus_address_;
            break;
        case SET_REG_BC_FROM_ADDRESS_BUS_:
            z80->internal.registers.BC = z80->internal.bus_address_;
            break;
        case SET_REG_SP_FROM_ADDRESS_BUS_:
            z80->internal.registers.SP = z80->internal.bus_address_;
            break;
        case SET_REG_IX_FROM_ADDRESS_BUS_:
            z80->internal.registers.IX = z80->internal.bus_address_;
            break;
        case SET_REG_IY_FROM_ADDRESS_BUS_:  
            z80->internal.registers.IY = z80->internal.bus_address_;
            break;
        case SET_REG_WZ_FROM_ADDRESS_BUS_:
            z80->internal.registers.WZ = z80->internal.bus_address_;
            break;
        case EXCHANGE_DE_HL:
            {
                uint16_t tmp = z80->internal.registers.DE;
                z80->internal.registers.DE = z80->internal.registers.HL;
                z80->internal.registers.HL = tmp;
            }
            break;
        case EXCHANGE_AF_AF_:
            {
                uint16_t tmp = z80->internal.registers.AF;
                z80->internal.registers.AF = z80->internal.registers.AF_;
                z80->internal.registers.AF_ = tmp;
            }
            break;
        case EXX:
            {
                uint16_t tmp = z80->internal.registers.BC;
                z80->internal.registers.BC = z80->internal.registers.BC_;
                z80->internal.registers.BC_ = tmp;
                tmp = z80->internal.registers.DE;
                z80->internal.registers.DE = z80->internal.registers.DE_;
                z80->internal.registers.DE_ = tmp;
                tmp = z80->internal.registers.HL;
                z80->internal.registers.HL = z80->internal.registers.HL_;
                z80->internal.registers.HL_ = tmp;
            }
            break;
        case INC_REG_R:
            z80->internal.registers.R = (0x80 & (z80->internal.registers.R)) | ((z80->internal.registers.R + 1) & 0x7F);
            break;
        case SET_REG_W_BASED_ON_ALU_S_FLAG:
            {
                if(z80->internal.alu.flags & FLAG_S_B)
                    z80->internal.registers.W = 0xFF;
                else
                    z80->internal.registers.W = 0x00;
            }
            break;
        case SET_REG_W_0:
            z80->internal.registers.W = 0x00;
            break;
        
        //ADDRESS
        case OUT_ADDRESS_BUS:
            z80->pin_state.address = z80->internal.bus_address;
            break;
        case INC_ADDRESS_BUS:
            z80->internal.bus_address_ = z80->internal.bus_address + 1;
            break;
        case DEC_ADDRESS_BUS:
            z80->internal.bus_address_ = z80->internal.bus_address - 1;
            break;
        case ID_ADDRESS_BUS:
            z80->internal.bus_address_ = z80->internal.bus_address;
            break;
        case SET_ADDRESS_BUS_FROM_IR:
            z80->internal.bus_address = z80->internal.registers.IR;
            break;
        case SET_ADDRESS_BUS_FROM_PC:
            z80->internal.bus_address = z80->internal.registers.PC;
            break;
        case SET_ADDRESS_BUS_FROM_HL:
            z80->internal.bus_address = z80->internal.registers.HL;
            break;
        case SET_ADDRESS_BUS_FROM_BC:
            z80->internal.bus_address = z80->internal.registers.BC;
            break;
        case SET_ADDRESS_BUS_FROM_DE:
            z80->internal.bus_address = z80->internal.registers.DE;
            break;
        case SET_ADDRESS_BUS_FROM_SP:
            z80->internal.bus_address = z80->internal.registers.SP;
            break;
        case SET_ADDRESS_BUS_FROM_WZ:
            z80->internal.bus_address = z80->internal.registers.WZ;
            break;
        case SET_ADDRESS_BUS_FROM_IY:
            z80->internal.bus_address = z80->internal.registers.IY;
            break;
        case SET_ADDRESS_BUS_FROM_IX:
            z80->internal.bus_address = z80->internal.registers.IX;
            break;

        //INTERNAL CONTROL
        case CHECK_FOR_WAIT:
            if(z80->internal.control_in.wait == LOW){
                z80->internal.state.step_op = STEP_WAIT;
            }
            break;
        case CHECK_FOR_BUSREQ:
            if(z80->internal.control_in.busrq == LOW){
                z80->internal.state.pending_busreq = 1;
                z80->internal.state.pending_counter = 3;
            }
            break;
        case CANCEL_BUSREQ_CHECK:
            z80->internal.state.pending_busreq = 0;
            break;
        case CHANGE_STATE_TO_STANDARD_FETCH:
            {
                z80->internal.state.step_op = STEP_ZERO;
                z80->internal.state.STATE = STANDARD_FETCH;
                if(z80->internal.state.pending_nmi){
                    z80->internal.state.STATE = NMI;
                    z80->internal.state.pending_nmi = 0;
                }
            }
            break;
        case CHANGE_STATE_TO_FETCH:
            {
                z80->internal.state.step_op = STEP_ZERO;
                z80->internal.state.STATE = FETCH;
                if(z80->internal.state.pending_nmi){
                    z80->internal.state.STATE = NMI;
                    z80->internal.state.pending_nmi = 0;
                }
            }
            break;
        case CHANGE_STATE_TO_FETCH_NO_NMI:
            {
                z80->internal.state.step_op = STEP_ZERO;
                z80->internal.state.STATE = FETCH;
            }
            break;
        case CHANGE_STATE_TO_OPCODE:
            z80->internal.state.step_op = STEP_ZERO;
            if(z80->internal.state.STATE == STANDARD_FETCH)
                z80->internal.state.opcode = opcodes_first[z80->internal.registers.INST];
            else if(z80->internal.state.STATE == FETCH_ED)
                z80->internal.state.opcode = opcodes_ed[z80->internal.registers.INST];
            else if(z80->internal.state.STATE == FETCH_CB)
                z80->internal.state.opcode = opcodes_cb[z80->internal.registers.INST];
            else if(z80->internal.state.STATE == FETCH_DD)
                z80->internal.state.opcode = opcodes_dd[z80->internal.registers.INST];
            else if(z80->internal.state.STATE == FETCH_FD)
                z80->internal.state.opcode = opcodes_fd[z80->internal.registers.INST];
            else if(z80->internal.state.STATE == FETCH_FDCB)
                z80->internal.state.opcode = opcodes_fdcb[z80->internal.registers.INST];
            else if(z80->internal.state.STATE == FETCH_DDCB)
                z80->internal.state.opcode = opcodes_ddcb[z80->internal.registers.INST];
            if(z80->internal.state.opcode == NULL)
                z80->internal.state.opcode = &OPCODE_NOP;
            z80->internal.state.STATE = OPCODE;
            break;
        case CHANGE_STATE_TO_OPCODE_DDCB:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.opcode = opcodes_ddcb[z80->internal.registers.INST];
            if(z80->internal.state.opcode == NULL)
                z80->internal.state.opcode = &OPCODE_NOP;
            z80->internal.state.STATE = OPCODE;
            break;
        case CHANGE_STATE_TO_OPCODE_FDCB:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.opcode = opcodes_fdcb[z80->internal.registers.INST];
            if(z80->internal.state.opcode == NULL)
                z80->internal.state.opcode = &OPCODE_NOP;
            z80->internal.state.STATE = OPCODE;
            break;
        case CHANGE_STATE_TO_HALT:
            z80->internal.state.STATE = HALT;
            if(z80->internal.state.pending_nmi){
                    z80->internal.state.STATE = NMI;
                    z80->internal.state.pending_nmi = 0;
            }
            z80->internal.state.step_op = STEP_ZERO;
            break;
        case CHANGE_STATE_TO_FETCH_ED:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.STATE = FETCH_ED;
            break;
        case CHANGE_STATE_TO_FETCH_FD:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.STATE = FETCH_FD;
            break;
        case CHANGE_STATE_TO_FETCH_FDCB:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.STATE = FETCH_FDCB;
            break;
        case CHANGE_STATE_TO_FETCH_DD:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.STATE = FETCH_DD;
            break;
        case CHANGE_STATE_TO_FETCH_DDCB:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.STATE = FETCH_DDCB;
            break;
        case CHANGE_STATE_TO_FETCH_CB:
            z80->internal.state.step_op = STEP_ZERO;
            z80->internal.state.STATE = FETCH_CB;
            break;
        case CHANGE_STATE_COND_NON_ZERO:
            {
                if(z80->internal.registers.F & FLAG_Z_B == 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_COND_ZERO:
            {
                if(z80->internal.registers.F & FLAG_Z_B != 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_COND_NON_CARRY:
            {
                if(z80->internal.registers.F & FLAG_C_B == 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_COND_CARRY:
            {
                if(z80->internal.registers.F & FLAG_C_B != 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_COND_PARITY_ODD:
            {
                if(z80->internal.registers.F & FLAG_P_V_B == 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_COND_PARITY_EVEN:
            {
                if(z80->internal.registers.F & FLAG_P_V_B != 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            } 
            break;
        case CHANGE_STATE_COND_SIGN_POSITIVE:
            {
                if(z80->internal.registers.F & FLAG_S_B == 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_COND_SIGN_NEGATIVE:
            {
                if(z80->internal.registers.F & FLAG_S_B != 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_IF_ADDRESS_0_TO_STANDARD_FETCH:
            {
                if(z80->internal.bus_address == 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_IF_P_V_0_TO_STANDARD_FETCH:
            {
                if(z80->internal.registers.F & FLAG_P_V_B == 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case CHANGE_STATE_IF_Z_1_TO_STANDARD_FETCH:
            {
                if(z80->internal.registers.F & FLAG_Z_B != 0){
                    z80->internal.state.STATE = STANDARD_FETCH;
                    z80->internal.state.step_op = STEP_ZERO;
                }
            }
            break;
        case SET_IM_TO_0:
            z80->internal.state.interrupt_mode = IM0;
            break;
        case SET_IM_TO_1:
            z80->internal.state.interrupt_mode = IM1;
            break;
        case SET_IM_TO_2:
            z80->internal.state.interrupt_mode = IM2;
            break;


        case RESET_CPU:
            {
                z80->internal.state.interrupt_mode = IM0;
                z80->internal.registers.IR = 0x0000;
                z80->internal.registers.PC = 0x0000;
                z80->internal.registers.SP = 0xffff;
                z80->internal.registers.IFF1 = 0;
                z80->internal.registers.IFF2 = 0;
                z80->internal.state.opcode = &OPCODE_NOP;
                z80->internal.registers.AF = 0x0000;
                z80->internal.control_out.halt = HIGH;
                z80->internal.control_out.busack = HIGH;
                z80->internal.control_out.m1 = HIGH;
                z80->internal.control_out.mreq = HIGH;
                z80->internal.control_out.rd = HIGH;
                z80->internal.control_out.wr = HIGH;
                z80->internal.control_out.rfsh = HIGH;
                z80->internal.control_out.iorq = HIGH;
                z80->internal.state.pending_busreq = 0;
                z80->internal.state.pending_nmi = 0;
                z80->internal.state.pending_counter = 0;
            }
            break;
        case SKIP_INSTRUCTION:
            break;
        default:
            printf("Unknown micro_code: %d\n", micro_code);
            break;
    }
}