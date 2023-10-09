#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t DataT;
typedef uint16_t AddressT;
typedef uint8_t ControlT;
typedef enum z80_identity{
    Z80_EMULATOR,
    Z80_HARDWARE
} z80_identity;
typedef enum {LOW = 0b0, HIGH = 0b1} SignalStateT;

typedef struct control_in_s{
    SignalStateT wait     : 1;
    SignalStateT interupt : 1;
    SignalStateT nmi      : 1;
    SignalStateT busrq    : 1;
    SignalStateT reset    : 1;
    SignalStateT padding  : 3;
} control_in_s;
typedef struct control_out_s{
    SignalStateT m1     : 1;
    SignalStateT mreq   : 1;
    SignalStateT iorq   : 1;
    SignalStateT rd     : 1;
    SignalStateT wr     : 1;
    SignalStateT rfsh   : 1;
    SignalStateT halt   : 1;
    SignalStateT busack : 1;
} control_out_s;

#endif //TYPES_H
