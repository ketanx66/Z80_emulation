#ifndef PINS_H
#define PINS_H

#define DDR_ADDRES_L  DDRK
#define PORT_ADDRES_L PORTK
#define PIN_ADDRES_L  PINK
#define P_A0 PK0
#define P_A1 PK1
#define P_A2 PK2
#define P_A3 PK3
#define P_A4 PK4
#define P_A5 PK5
#define P_A6 PK6
#define P_A7 PK7

#define DDR_ADDRES_H  DDRC
#define PORT_ADDRES_H PORTC
#define PIN_ADDRES_H  PINC
#define P_A8  PC8
#define P_A9  PC9
#define P_A10 PC10
#define P_A11 PC11
#define P_A12 PC12
#define P_A13 PC13
#define P_A14 PC14
#define P_A15 PC15

#define DDR_DATA  DDRA
#define PORT_DATA PORTA
#define PIN_DATA  PINA
#define P_D0 PA0
#define P_D1 PA1
#define P_D2 PA2
#define P_D3 PA3
#define P_D4 PA4
#define P_D5 PA5
#define P_D6 PA6
#define P_D7 PA7

#define DDR_CONTROL  DDRF
#define PORT_CONTROL PORTF
#define PIN_CONTROL  PINF
#define P_MREQ   PF0
#define P_IORQ   PF1
#define P_RD     PF2
#define P_WR     PF3
#define P_BUSACK PF4
#define P_HALT   PF5 
#define P_M1     PF6
#define P_RFSH   PF7

#define DDR_INPUT  DDRB
#define PORT_INPUT PORTB
#define PIN_INPUT  PINB
#define P_INT    PB4
#define P_NMI    PB5
#define P_BUSREQ PB6
#define P_WAIT   PB7

#define DDR_RESET  DDRH
#define PORT_RESET PORTH
#define PIN_RESET  PINH
#define P_RESET PH6

#define DDR_CLOCK  DDRG
#define PORT_CLOCK PORTG
#define PIN_CLOCK  PING
#define P_CLOCK PG5

#endif