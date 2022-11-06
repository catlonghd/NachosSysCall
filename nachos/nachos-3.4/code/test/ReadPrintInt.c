#include"syscall.h"

int main()
{
    int test;
    test = ReadInt();
    PrintInt(test);

    Halt();
}