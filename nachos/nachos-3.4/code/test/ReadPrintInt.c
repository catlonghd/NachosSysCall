#include"syscall.h"

int main()
{
    int test;
    PrintString("Nhap so: ");
    test = ReadInt();
    PrintString("So da nhap: ");
    PrintInt(test);
    PrintChar('\n');
    Halt();
}
