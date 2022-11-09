#include"syscall.h"

int main()
{
    int test;
    PrintString("Nhap ky tu: ");
    test = ReadChar();
    PrintString("Ky tu da nhap: ");
    PrintChar(test);

    Halt();
}