#ifndef EMULATOR_H
#define EMULATOR_H

#include <types.h>
#include <constants.h>
#include <stddef.h>
#include <z80.h>
#include <micro_code.h>


void emulator_init(z80_t* cpu);
void emulator_tick(z80_t* cpu);

void execute_micro_code(z80_t *z80, micro_code_t micro_code);

#endif //EMULATOR_H
