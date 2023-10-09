#ifndef CONTROLS_H
#define CONTROLS_H

#define M1     ((PIN_CONTROL & _BV(P_M1))     == 0)
#define MREQ   ((PIN_CONTROL & _BV(P_MREQ))   == 0)
#define IORQ   ((PIN_CONTROL & _BV(P_IORQ))   == 0)
#define RD     ((PIN_CONTROL & _BV(P_RD))     == 0)
#define WR     ((PIN_CONTROL & _BV(P_WR))     == 0)
#define RFSH   ((PIN_CONTROL & _BV(P_RFSH))   == 0)
#define HALT   ((PIN_CONTROL & _BV(P_HALT))   == 0) 
#define BUSACK ((PIN_CONTROL & _BV(P_BUSACK)) == 0)


//Create defines to clear and set pin in port
#define CREATE_SET_CLEAR(PORT, PIN) \
  static inline void SET_##PORT##_##PIN(uint8_t state) { \
    if(state == 0) { \
      PORT &= ~_BV(PIN); \
    } else { \
      PORT |= _BV(PIN); \
    } \
  }

CREATE_SET_CLEAR(PORT_INPUT, P_INT)
CREATE_SET_CLEAR(PORT_INPUT, P_NMI)
CREATE_SET_CLEAR(PORT_INPUT, P_BUSREQ)
CREATE_SET_CLEAR(PORT_INPUT, P_WAIT)
CREATE_SET_CLEAR(PORT_RESET, P_RESET)
CREATE_SET_CLEAR(PORT_CLOCK, P_CLOCK)


#endif