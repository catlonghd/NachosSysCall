/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

#define MAX 100

int main()
{
    int n = 0;
    int a[MAX];
    PrintString("Nhap kich thuoc mang: ");
    n = ReadInt();
    while(n <= 0 || n > MAX){
        PrintString("Do dai khong hop le, moi nhap lai\n");
        PrintString("Nhap kich thuoc mang: ");
        n = ReadInt();
    }

    PrintString("Nhap mang:\n");
    for(int i = 0; i<n; i++){
        PrintString("a[");
        PrintInt(i);
        PrintString("]: ");
        a[i] = ReadInt();
    }

    PrintString("Mang ban dau: ");
    for(int i = 0; i<n; i++){
        PrintInt(a[i]);
    }
    PrintChar('\n');

    int temp; // biến tạm temp
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            if (a[j] > a[j+1]){
                temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
		}
	}

    PrintString("Mang sau khi da sap xep: ");
    for(int i = 0; i<n; i++){
        PrintInt(a[i]);
    }
    PrintChar('\n');

    Halt();
}
