#include "syscall.h"


int main(){
    int i;
    PrintString("---ASCII TABLE FROM [32, 126]---\n");

    PrintString("DEC\tCHAR\n");
    for(i = 32; i <= 126; i++){
        PrintInt(i);
        PrintChar('\t');
        PrintChar((char)i);
        PrintChar('\n');
    }

    Halt();
}
