#include "mem.h"
#include "logging.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Arg ss, dd;
word nn, r;
char Z, N, V, C;
signed char xx;

Command cmd[] = {
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
    {0170000, 0110000, "movb", do_mov, HAS_SS | HAS_DD},
    {0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
    {0177000, 0077000, "sob", do_sob, HAS_NN | HAS_RL},
    {0177000, 0005000, "clr", do_clr, HAS_DD},
    {0177400, 0001400, "beq", do_beq, HAS_XX},
    {0177400, 0001000, "bne", do_bne, HAS_XX},
    {0177400, 0100400, "bmi",   do_bmi,  HAS_XX},
    {0177400, 0000400, "br", do_br, HAS_XX},
    {0177400, 0100000, "bpl", do_bpl, HAS_XX},
    {0177700, 0000100, "jmp", do_jmp, HAS_DD},
    {0170000, 0120000, "cmpb",  do_cmpb, HAS_SS | HAS_DD},
    {0170000, 0020000, "cmp",   do_cmp,  HAS_SS | HAS_DD},
    {0177700, 0105700, "tstb", do_tstb, HAS_DD},
    {0177700, 0005700, "tst",  do_tst,  HAS_DD},
    {0000000, 0000000, "nothing", do_nothing, NO_PARAMS}   // Всегда последняя команда
};


void check_sign_bit() {    // проверка знакового бита
    char bit_7 = (ss.val >> 7) & 1;
    	if (bit_7 == 1) {
        
        w_write(dd.adr, ss.val | 0xFF00);
 
    }
    else {
        b_write(dd.adr, (byte)ss.val);
    }
}

char get_nn (word w) {
    return w & 077;
}

int get_r (word w) {
    r = w & 07;
    printf("R%d ", r);
    return r;
}

char get_xx (word w) {
    return (byte)(w & 0xFF);
}

void set_NZ(word w) {
    if (w == 0) {
        Z = 1;
        N = 0;
    } else if (w < 0) {
        N = 1;
        Z = 0;
    } else {
        N = 0;
        Z = 0;
    }
}
void set_C(int x) {
    if ((x >> 16) && (x > 0))
        C = 1;
    else
        C = 0;
}

void do_halt(){
    trace(TRACE, "THE END!!!\n");
    reg_print(TRACE);
    exit(0);
}
void do_mov(){
    if (is_byte_cmd){
        check_sign_bit();
        
    }
    else {
        w_write(dd.adr, ss.val);     
    }
    set_NZ(ss.val);
    
}
  
void do_add(){
    w_write(dd.adr, ss.val + dd.val);
}

void do_sob() {
    //reg[r] --;
    //printf("nn = %o ", nn);
    if (--reg[r] != 0)
        pc = pc - (2 * nn);
    //printf("%06o ", pc);

} 

void do_clr() {
    w_write(dd.adr, 0);
}

void do_tst() {
    unsigned int val = w_read(dd.adr);
    set_NZ(w_read(dd.adr));
    //printf("dd.a = %06o val = %06o b = %d ss.val = %06o", dd.adr, val, b, ss.val);

}

void do_tstb() {
    unsigned int val = b_read(dd.adr);
    set_NZ(b_read(dd.adr));
    //printf("dd.a = %06o val = %06o b = %d ss.val = %06o", dd.adr, val, b, ss.val);
}

void do_cmp() {
    w_write(dd.adr, ss.val - dd.val);
    set_NZ(ss.val - dd.val);
}

void do_cmpb() {
    b_write(dd.adr, ss.val - dd.val);
    set_NZ(ss.val - dd.val);
}
// dung de nhay den dc cần do_br
void do_br() {
    pc = pc + 2 * xx;
}

void do_beq() {
    if(Z == 1)
        do_br();
}

void do_bne() {
    if(Z == 0)
        do_br();
}

void do_bmi() {
    if(N == 1)
        do_br();
}

void do_bpl() {
    if(N == 0)
        do_br();
}

void do_jmp() {
	pc = dd.val;
}
void do_nothing(){
    trace(ERROR, "Unknown command\n");
    exit(1);
}


Arg get_mr(int byte, word w) {
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
            if(byte == 0)
                res.val = w_read(res.adr);
            else                
                res.val = b_read(res.adr); //b_read
            trace(TRACE, "(R%o) ", r);
            break; 
        // (Rn3)+ #3, мода 2,chế độ địa chỉ tăng tự động автоинкрементный режим адресации    
        case 2:   
            res.adr = reg[r];
            if (byte == 0)
                res.val = w_read(res.adr);
            else
                res.val = b_read(res.adr);
            if (byte && r < 6)        
                reg[r] ++;               // todo +1
            else 
                reg[r] += 2;    
            if (r == 7)                // nếu địa chỉ = 7, thì in ra gtri do thôi
                trace(TRACE, "#%o ", res.val);
            else
                trace(TRACE, "(R%o)+ ", r);    
            break;
        // @(Rn)+, chế độ 3, chế độ địa chỉ tăng tự động gián tiếp    
        case 3:
            res.adr = reg[r];
            reg[r] += 2;
            res.adr = w_read(res.adr);
            //res.val = w_read(res.adr);
            if (byte == 0)
                res.val = w_read(res.adr); 
            else
                res.val = b_read(res.adr);
            if (r == 7)
                printf("@#%o ", res.adr );
            else
                printf("@(R%d) ", r);
            break;    
        
        // -(Rn), мода 4,  chế độ tự động giảm автодекрементный режим    
        case 4:
            if (byte && r <6)
                reg[r] --;      // -1 byte r1 -> r5
            else    
                reg[r] -= 2;    // -2 word r6, r7
            res.adr = reg[r];
            if (byte == 0)
                res.val = w_read(res.adr); 
            else
                res.val = b_read(res.adr);
            if (r == 0)                // nếu địa chỉ = 7, thì in ra gtri do thôi
                trace(TRACE, "#%o ", res.val);
            else
                trace(TRACE, "-(R%o) ", r);    
            break;
        //Chế độ @-(Rn) 5, chế độ định địa chỉ tự động giảm gián tiếp
        case 5:
            reg[r] -= 2;        
            res.adr = reg[r];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            if (r == 7)
                printf("@#%o ", res.adr );
            else
                printf("@(R%d) ", r);
            break;    

        // khi sai thi cac buoc sau ko lm nua    
        default:
            fprintf(stderr, "Mode %o NOT IMPLEMENTED YET!\n", mode);
            exit (1);           
    }

    return res;
}




