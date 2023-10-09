#include <stdio.h>

extern void main_test_z80();
extern void main_test_bus();
extern void main_test_memory();
extern void main_test_memory_bus();

int main(){
    main_test_z80();
    main_test_bus();
    main_test_memory();
    main_test_memory_bus();
    return 0;
}