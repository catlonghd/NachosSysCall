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
//		arg1 -- r4s
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
                char* buffer = new char[256];
                int len;
                bool check = true;

                len = gSynchConsole->Read(buffer, 256);
                int i = 0;

                if(buffer[0] == '-')
                    i++;

                for(; i < len; i++) 
                {
                    if(buffer[i] < '0' || buffer[i] > '9')
                    {
                        machine->WriteRegister(2,0);
                        printf("\nNot an Integer\n");
                        check = false;
                        break;
                    }
                }   

                if(!check)
                    break;
                int result = atoi(buffer);

                //machine->WriteRegister(buffer);

                machine->IncreaseProgramCounter();
                break;
            }
            case SC_PrintInt:
            {
                int number = machine->ReadRegister(4);
                if(number == 0)
                {
                    gSynchConsole->Write("0", 1);
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
            case SC_ReadString:
            {   
                int virtAddr = machine->ReadRegister(4);
                int len = machine->ReadRegister(5);
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

                while(len < MAX_BUFFER_LENGTH && buffer[len] != NULL) len++;

                if(len == MAX_BUFFER_LENGTH){
                    DEBUG('a', "Error: String length cannot over 254 characters");
                }
                else {
                    gSynchConsole->Write(buffer, len + 1);
                }

                machine->IncreaseProgramCounter();
                break;  
            }
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
