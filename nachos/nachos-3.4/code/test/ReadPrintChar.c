#include"syscall.h"

int main()
{
    int test;
    PrintString("Nhap ky tu: ");
    test = ReadInt();
    PrintString("So da nhap: ");
    PrintInt(test);

    Halt();
}