#include"syscall.h"
#define MAX_BUFFER_LENGTH 254

int main()
{
    char* test[MAX_BUFFER_LENGTH - 1];
    PrintString("Nhap vao chuoi: ");
    ReadString(test, MAX_BUFFER_LENGTH);
    PrintString("Chuoi da nhap: ");
    PrintString(test);
    PrintString("\n");

    Halt();
}
