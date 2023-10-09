CC = gcc

EXEC := main.out
TEXEC := test.out

DIR_SRC := ./src
DIR_INC := ./include
DIR_TEST := ./test
DIR_GEN := ./code_generator

ifeq ($(debug),on)
  DEBUG := -DDEBUG -g -Og
else
  DEBUG := -O3
endif

ifeq ($(log_pins),on)
  LOG += -DLOG_PINS
else
  LOG :=
endif

endian ?= $(shell echo -n I | od -to2 | head -n1 | awk '{ print substr($$0, length($$0) - 0, 1) }')
ifeq ($(endian),0)
ENDIAN := -DBIG_ENDIAN
else
ENDIAN := -DLITTLE_ENDIAN
endif

DEFINES := ${DEBUG} ${LOG} ${ENDIAN}



all:
	cd $(DIR_GEN) && python3 generate_opcodes.py
	mv -f $(DIR_GEN)/opcodes.h $(DIR_INC)/opcodes.h
	mv -f $(DIR_GEN)/opcodes.c $(DIR_SRC)/opcodes.c
	$(CC) $(DIR_SRC)/*.c -I$(DIR_INC) $(DEFINES) -o $(EXEC) 

test:
	$(CC) $(DIR_SRC)/*.c $(DIR_TEST)/*.c -I$(DIR_INC) -DTEST_BUILD $(DEFINES) -o $(TEXEC)

.PHONY: test clean
clean:
	rm -rf *.o
	rm -rf $(EXEC)
	rm -rf $(TEXEC)
	rm -rf memdump.bin
	rm -rf log*.txt
	rm -rf .gdb_history