#include "mem.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    word val; // значение аргумента
    word adr; // aдрес аргумента

} Arg;
Arg ss, dd;

void do_halt(){
    trace(TRACE, "THE END!!!\n");
    exit(0);
}
void do_mov(){
    w_write(dd.adr, ss.val);  //vt vao dau & lay tu dau

}
void do_add(){
    w_write(dd.adr, ss.val + dd.val);
}
void do_nothing(){}
typedef struct{
    word mask;
    word opcode;
    char * name;
    void (*do_func)(void);

} Command;
Command cmd[] = {
    {0170000, 0010000, "mov", do_mov},
    {0170000, 0060000, "add", do_add},
};

Arg get_mr(word w) {
    Arg res;
    int r = w & 7; // номер регистра -3 bit nen la so 7, reg=w&7;
    int mode =(w >> 3) & 7; //3 so sau
    switch(mode) {
        // R3 mode 0 значение находится в указанном регистре
        case 0:   
            res.adr = r;
            res.val = reg[r];
            trace(TRACE, "R%o ", r);
            break;
        // (R3) mode 1 một thanh ghi, truy cập vào bộ nhớ    
        case 1:   
            res.adr = reg[r];
            res.val = w_read(res.adr); //b_read
            trace(TRACE, "(R%o) ", r);
            break; 
        // (Rn3)+ #3, мода 2,chế độ địa chỉ tăng tự động автоинкрементный режим адресации    
        case 2:   
            res.adr = reg[r];
            res.val = w_read(res.adr); // to_do b_read
            reg[r] += 2;               // todo +1
            if (r == 7)                // nếu địa chỉ = 7, thì in ra gtri do thôi
                trace(TRACE, "#%o ", res.val);
            else
                trace(TRACE, "(R%o)+ ", r);    
            break;
        // -(Rn), мода 4,  chế độ tự động giảm автодекрементный режим    
        case 4:
            reg[r] -= 2;
            res.adr = reg[r];
            res.val = w_read(res.adr); // to_do b_read
            if (r == 0)                // nếu địa chỉ = 7, thì in ra gtri do thôi
                trace(TRACE, "#%o ", res.val);
            else
                trace(TRACE, "-(R%o) ", r);    
            break;

        // khi sai thi cac buoc sau ko lm nua    
        default:
            fprintf(stderr, "Mode %o NOT IMPLEMENTED YET!\n", mode);
            exit (1);           
    }

    return res;
}


void run(){
     pc = 01000;
     while(1){
        word w = w_read(pc); 
        trace(TRACE, "%06o %06o", pc, w);
        pc +=2;
        if (w == 0){
            trace(TRACE, "halt ");
            do_halt();
        }
        if ((w & 0xF000) == 0010000){      //01ssdd
            trace(TRACE, "mov ");
            do_mov();
        }

     }
}
