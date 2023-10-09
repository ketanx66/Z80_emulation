#ifndef Z80_PRIV_H
#define Z80_PRIV_H

#include <types.h>
#include <stddef.h>
#include <stdio.h>
#include <z80_opcodes.h>

typedef struct z80_registers_s{
    struct{
        union {
            struct {
                #ifdef LITTLE_ENDIAN
                uint8_t A; uint8_t F;
                #else
                uint8_t F; uint8_t A;
                #endif
            }; 
            uint16_t AF;
        };
    };
    struct{
        union { 
            struct {
                #ifdef LITTLE_ENDIAN
                uint8_t C; uint8_t B;
                #else
                uint8_t B; uint8_t C;
                #endif
            }; 
            uint16_t BC;
        };
    };
    struct{
        union { 
            struct {
                #ifdef LITTLE_ENDIAN
                uint8_t E; uint8_t D;
                #else
                uint8_t D; uint8_t E;
                #endif
            }; 
            uint16_t DE;
        };
    };
    struct{
        union { 
            struct {
                #ifdef LITTLE_ENDIAN
                uint8_t L; uint8_t H;
                #else
                uint8_t H; uint8_t L;
                #endif
            };
            uint16_t HL;
        };
    };
    struct{
        union {
            struct {
                #ifdef LITTLE_ENDIAN
                uint8_t Z; uint8_t W;
                #else
                uint8_t W; uint8_t Z;
                #endif
            };
            uint16_t WZ;
        };
    };
    struct{
        union {
            struct{
                #ifdef LITTLE_ENDIAN
                uint8_t IXL; uint8_t IXH;
                #else
                uint8_t IXH; uint8_t IXL;
                #endif
            };
            uint16_t IX;
        };
    };
    struct{
        union{
            struct{
                #ifdef LITTLE_ENDIAN
                uint8_t IYL; uint8_t IYH;
                #else
                uint8_t IYH; uint8_t IYL;
                #endif
            };
            uint16_t IY;
        };
    };
    struct{
        union{
            struct{
                #ifdef LITTLE_ENDIAN
                uint8_t SPL; uint8_t SPH;
                #else
                uint8_t SPH; uint8_t SPL;
                #endif
            };
            uint16_t SP;
        };
    };
    struct{
        union{
            struct{
                #ifdef LITTLE_ENDIAN
                uint8_t PCL; uint8_t PCH;
                #else
                uint8_t PCH; uint8_t PCL;
                #endif
            };
            uint16_t PC;
        };
    };
    struct{
        union{
            struct{
                #ifdef LITTLE_ENDIAN
                uint8_t R; uint8_t I;
                #else
                uint8_t I; uint8_t R;
                #endif
            };
            uint16_t IR;
        };
    };
    uint16_t AF_;
    uint16_t BC_;
    uint16_t DE_;
    uint16_t HL_;
    uint8_t IFF1:1;
    uint8_t IFF2:1;
    uint8_t :6;
    uint8_t TMP;
    uint8_t ACU;
    uint8_t INST;
} z80_registers_s;

typedef enum z80_registers_enum{
    A,
    F,
    AF,
    B,
    C,
    BC,
    D,
    E,
    DE,
    H,
    L,
    HL,
    W,
    Z,
    WZ,
    IXH,
    IXL,
    IX,
    IYH,
    IYL,
    IY,
    SPH,
    SPL,
    SP,
    PCH,
    PCL,
    PC,
    I,
    R,
    IR,
    AF_,
    BC_,
    DE_,
    HL_,
    IFF1,
    IFF2,
    TMP,
    ACU,
    INST
} z80_registers_enum;

typedef enum flag_bits{
    FLAG_C_B = 0x01,
    FLAG_N_B = 0x02,
    FLAG_P_V_B = 0x04,
    FLAG_X_B = 0x08,
    FLAG_H_B = 0x10,
    FLAG_Y_B = 0x20,
    FLAG_Z_B = 0x40,
    FLAG_S_B = 0x80
} flag_bits;

typedef struct alu_s{
    uint8_t flags;
    uint8_t saved_flags;
    uint8_t saved_flags_state;
} alu_s;

typedef struct z80_internal_s{
    z80_registers_s registers;
    DataT bus_data;
    AddressT bus_address;
    AddressT bus_address_;
    control_in_s control_in;
    control_out_s control_out;
    alu_s alu;
    states state;
} z80_internal_s;

typedef struct z80_pin_state_s{
    AddressT address;
    DataT data;
    control_out_s control_out;
    control_in_s control_in;
}z80_pin_state_s;

typedef size_t z80_clock_t;

typedef struct z80_t
{
    z80_internal_s internal;
    z80_pin_state_s pin_state;
    FILE* uart_file;
#ifdef LOG_PINS
    FILE* log_file;
#endif
    z80_clock_t clock;
    size_t cpu_id;
    z80_identity identity;
}z80_t;



#define _BV(bit) (1 << (bit))

#define _set_flag(reg, bit) (reg |= _BV(bit))
#define _clear_flag(reg, bit) (reg &= ~_BV(bit))
#define _toggle_flag(reg, bit) (reg ^= _BV(bit))
#define _get_flag(reg, bit) (reg & _BV(bit))
#define _set_flag_value(reg, bit, value) (value ? _set_flag(reg, bit) : _clear_flag(reg, bit))


#endif //Z80_PRIV_H
