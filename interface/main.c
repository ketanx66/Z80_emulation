#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <string.h>
#include "pins.h"
#include "controls.h"
#include <stdlib.h>

#define F_CPU 16000000
#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define CLOCK_HIGH 1
#define CLOCK_LOW  0
#define HIGH 0
#define LOW 1

void uart_init()
{
  UBRR0 = UBRR_VALUE;
  UCSR0A = 0;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

int uart_receive(FILE *stream)
{
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

int uart_putchar(char c, FILE *stream)
{
  uart_transmit(c, stream);
  return 0;
}

FILE uart_file;

inline uint8_t get_DATA() {
	// PORT_DATA = 0xff;
  DDR_DATA = 0x00;
	return PIN_DATA;
}

inline void put_DATA(uint8_t data) {
  DDR_DATA = 0xff;
  PORT_DATA = data;
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  // TURN resisors on on input pins


  DDR_ADDRES_L = 0x00;
  DDR_ADDRES_H = 0x00;
  DDR_DATA     = 0x00;
  DDR_CONTROL  = 0x00;
  PORT_ADDRES_H = 0xff;
  PORT_ADDRES_L = 0xff;
  PORT_CONTROL  = 0xff;
  PORT_DATA = 0x00;

  DDR_INPUT    = 0x00;
  DDR_INPUT  |= _BV(P_INT) | _BV(P_NMI) | _BV(P_BUSREQ) | _BV(P_WAIT);
  PORT_INPUT |= _BV(P_INT) | _BV(P_NMI) | _BV(P_BUSREQ) | _BV(P_WAIT);

  DDR_CLOCK |= _BV(P_CLOCK);

  DDR_RESET |= _BV(P_RESET);
  PORT_RESET &= ~_BV(P_RESET);
  

  enum {
    wait_bit=0,
    int_bit=1,
    nmi_bit=2,
    reset_bit=3,
    busrq_bit=4,
    clock_bit=5
  };

  {//Test connection
    uint8_t test = 0;
    test = getchar();
    putchar(test);
    test = 74;
    putchar(test);
    uint8_t test_back;
    test_back = getchar();
    if(test + 1 != test_back)
      while(1){};
    putchar(test_back + 1);
  }

  while(1) { 
    uint8_t data = 0;
    uint8_t control = 0;
    control = getchar();
    data = getchar();

    SET_PORT_INPUT_P_WAIT(control & _BV(wait_bit));
    SET_PORT_INPUT_P_INT(control & _BV(int_bit));
    SET_PORT_INPUT_P_NMI(control & _BV(nmi_bit));
    SET_PORT_RESET_P_RESET(control & _BV(reset_bit));
    SET_PORT_INPUT_P_BUSREQ(control & _BV(busrq_bit));

    if(RD){
      put_DATA(data);
    }else{
      data = get_DATA();
    }

    uart_putchar(PIN_CONTROL, &uart_file);
    uart_putchar(PIN_ADDRES_H, &uart_file);
    uart_putchar(PIN_ADDRES_L, &uart_file);
    uart_putchar(data, &uart_file);
    SET_PORT_CLOCK_P_CLOCK(control & _BV(clock_bit));

  } //while
}
