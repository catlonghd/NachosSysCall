#include "syscall.h"

int main(){
    PrintString("- Thong tin thanh vien nhom\n");
    PrintString("\t+20120264 - Tran Hai Dang\n");
    PrintString("\t+20120257 - Dinh Hoang Bao Chau\n");
    PrintString("\t+20120258 - Lam Quoc Chung\n");
    PrintString("\t+20120281 - Ngo Thanh Hai\n");
    PrintString("\t+20120335 - Cai Huu Nghia\n");


    PrintString("- Mo ta chuong trinh ascii\n");
    PrintString("\tChuong trinh ascii cho phep in ra gia tri decimal cua mot ki tu và in ra ky tu tuong ung voi ma ascii do\n");
    PrintString("\tChuong trinh chi in cac ky tu trong khoang [32,126] vi day la cac ky co the in ra duoc\n");


    PrintString("- Mo ta chuong trih sort\n");
    PrintString("\tDau tien chuong trinh cho phep nguoi dung nhap vao kich thuoc cua mang, neu kich thuoc lon hon 100 hoac nho hon 0 thi yeu cau nhap lai\n");
    PrintString("\tSau do chuong trinh cho phep nguoi dung nhap vao cac phan tu cua mang\n");
    PrintString("\tChuong trinh se sap xep mang theo thu tu tang dan bang thuat toan bubble sort\n");
    PrintString("\tSau khi sap xep chuong trinh se in ra mang ban dau va mang sau khi da sap xep\n");

    Halt();
}