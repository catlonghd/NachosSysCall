// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "cmath"


#define MAX_BUFFER_LENGTH 255
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void 
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    int op1, op2, result;
    int size;
    char s[200];
    char* s2;
    int a;

    //ReadInt
    char* buffer = new char[256];
    int len;
    bool check = true;

    switch (which)
    {
    case NoException:
        return;
    case PageFaultException:
        DEBUG('a', "\nNo valid translation found");
        printf("No valid translation found");
        interrupt->Halt();
        break;
    
    case SyscallException:
        switch(type){
            case SC_Halt:
                DEBUG('a', "Shutdown, initialated by user program.\n");
                interrupt->Halt();
                break;
            case SC_ReadInt:
            {
                char* buffer = new char[MAX_BUFFER_LENGTH + 1];
                int len;
                bool check = true;

                len = gSynchConsole->Read(buffer, MAX_BUFFER_LENGTH);
                int i = 0;


		//Chuoi rong
                if(len == 0){
		            DEBUG('a', "Not an integer number");
                    machine->WriteRegister(2, 0);
		    
                }
                else{
		    //Neu ky tu dau la - thi tang index them 1
                    if(buffer[0] == '-')
                    i++;


		    //Duyet qua tung ky tu de kiem tra cac ky tu co phai la so hay khong
                    for(; i < len; i++) 
                    {
			//Neu khong phai ky tu so thi ghi vao bo nho 0
                        if(buffer[i] < '0' || buffer[i] > '9')
                        {
			    DEBUG('a', "Not an integer number");
                            machine->WriteRegister(2,0);
                            check = false;
                            break;
                        }
                    }


	            //Sau khi kiem tra neu la so hop le thi ghi ket qua vao vung nho
                    if(check)
                    {   
                        int result = atoi(buffer);
 
                        machine->WriteRegister(2, result);
		    }
                }

                machine->IncreaseProgramCounter();
                break;
            }
            case SC_PrintInt:
            {
                int number = machine->ReadRegister(4);

		//Neu so doc duoc la 0 thi tang program counter va dung chuong trinh
                if(number == 0)
                {
                    gSynchConsole->Write("0", 1);
		    machine->IncreaseProgramCounter();
                    break;
                }

                bool sign = number < 0 ? 1 : 0;
                number *= 1 - sign * 2;

                int numLen = (int)log10(number) + 1;
                numLen += sign;

                char* toScreen = new char[numLen + 1];
                toScreen[numLen] = '\0';
                int i = numLen - 1;
                while(number) 
                {
                    toScreen[i] = (char)((number % 10) + '0');
                    number /= 10;
                    i--;
                }
                toScreen[0] = sign ? '-' : toScreen[0];

                gSynchConsole->Write(toScreen, numLen + 1);
                machine->IncreaseProgramCounter();
                break;
            }
            case SC_ReadChar:
            {  
                char* buffer = new char[MAX_BUFFER_LENGTH + 1];
                
                int numBytesRead = gSynchConsole->Read(buffer, MAX_BUFFER_LENGTH);
		
		//Neu chuo la rong thi ghi vao vung nho NULL
                if(numBytesRead == 0){
                    machine->WriteRegister(2, 0);
                }
                else{
                    char readChar = buffer[0];
                    machine->WriteRegister(2, readChar);
                }

                machine->IncreaseProgramCounter();
                break;
                
            }
            case SC_PrintChar:
            {
		char output = (char)machine->ReadRegister(4);

		//Neu nguoi dung nhap vao chuoi thi chi lay ky tu dau tien
                gSynchConsole->Write(&output, 1);

                machine->IncreaseProgramCounter();
                break;
            }
            case SC_ReadString:
            {   
                int virtAddr = machine->ReadRegister(4);
                int len = machine->ReadRegister(5);
		
		//Copy chuoi vua nhap vao vung nho he thong
                char* buffer = machine->User2System(virtAddr, len);
                gSynchConsole->Read(buffer, len);
                machine->System2User(virtAddr, len, buffer);
                
                delete[] buffer;

                machine->IncreaseProgramCounter();
                break;
            }
            case SC_PrintString:
            {
                int len = 0;
                char* buffer = machine->User2System(machine->ReadRegister(4), MAX_BUFFER_LENGTH);

		//Lay do dai cua chuoi
                while(len < MAX_BUFFER_LENGTH && buffer[len] != NULL) len++;
		
		//Do dai chuoi vuot qua do dai quy dinh
                if(len == MAX_BUFFER_LENGTH){
                    DEBUG('a', "Error: String length cannot over 254 characters");
                }
                else {
                    gSynchConsole->Write(buffer, len + 1);
                }

                machine->IncreaseProgramCounter();
                break;  
            }
	    default:
		printf("Unexpected user mode exception %d %d\n", which, type);
	    	ASSERT(FALSE);
		machine->IncreaseProgramCounter();
        }
        break;
    case ReadOnlyException:
        DEBUG('a', "\nWrite attempted to page  marked \"read-only\".");
        printf("\n\nWrite attempted to page  marked \"read-only\".");
        interrupt->Halt();
        break;
    case BusErrorException:
        DEBUG('a', "\nTranslation resulted in an invalid physical address.");
        printf("\n\nTranslation resulted in an invalid physical address.");
        interrupt->Halt();
        break;
    case AddressErrorException:
        DEBUG('a', "\nUnaligned reference or one that was beyond the end of the address space.");
        printf("\n\nUnaligned reference or one that was beyond the end of the address space.");
        interrupt->Halt();
        break;
    case OverflowException:
        DEBUG('a', "\nInteger overflow in add or sum.");
        printf("\n\nInteger overflow in add or sum.");
        interrupt->Halt();
        break;
    case IllegalInstrException:
        DEBUG('a', "\nUnimplemented or reserved instr.");
        printf("\n\nInteger overflow in add or sum.");
        interrupt->Halt();
        break;
    case NumExceptionTypes:
        break;
    default:
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(FALSE);
    }
}
