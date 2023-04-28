#include "mem.h"
#include "logging.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Arg ss, dd;
word nn, ri;
//int nn, ri;
int b_or_w;
char Z, N, V, C;  // flags
signed char xx;

Command cmd[] = {
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
    {0170000, 0110000, "movb", do_mov, HAS_SS | HAS_DD},
    {0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
    {0177700, 0005200, "inc ",  do_inc, HAS_DD},
    {0177000, 0077000, "sob", do_sob, HAS_RL | HAS_NN  },
    {0177000, 0005000, "clr", do_clr, HAS_DD},
    {0170000, 0120000, "cmpb", do_cmpb, HAS_SS | HAS_DD},
    {0170000, 0020000, "cmp", do_cmp,  HAS_SS | HAS_DD},
    {0177400, 0000400, "br", do_br, HAS_XX},
    {0177400, 0103000, "bcc", do_bcc, HAS_XX},
	{0177400, 0103400, "bcs", do_bcs, HAS_XX},
    {0177400, 0001400, "beq", do_beq, HAS_XX},
    {0177400, 0001000, "bne", do_bne, HAS_XX},
    {0177700, 0005700, "tst", do_tst,  HAS_DD},
    {0177700, 0105700, "tstb", do_tst, HAS_DD},
    {0177400, 0100400, "bmi", do_bmi,  HAS_XX},
    {0177400, 0100000, "bpl", do_bpl, HAS_XX},
    {0177000, 0004000, "jsr ",  do_jsr, HAS_DD | HAS_RL},
	{0177770, 0000200, "rts ",  do_rts, HAS_RE},
    {0177700, 0000100, "jmp", do_jmp, HAS_DD},
    {0000000, 0000000, "nothing", do_nothing, NO_PARAMS}   // Всегда последняя команда
};


void check_sign_bit() {    // проверка знакового бита
    //char bit_7 = (ss.val >> 7) & 1;
    int bit_7 = (ss.val >> 7) & 1;
    if (bit_7 == 1) {
        w_write(dd.adr, ss.val | 0xFF00);
    }
    else {
        b_write(dd.adr, (byte)ss.val);
    }
}
void check_NZ(word w)
{
	Z = w == 0; //флаг Z = 0, если w = 1
	if (b_or_w)
		N = (w >> 7) & 1;
	else
		N = (w >> 15) & 1;
}

void check_C(int w)
{
	if (b_or_w)
		C = (w >> 8) & 1;
	else 
		C = (w >> 16) & 1;
}

char get_nn (word w) {
    return w & 077;
}

int get_r (word w) {
    ri = w & 07;
    printf("R%d ", ri);
    return ri;
}

/*void get_xx(word w)
{
	xx = w;
	trace(TRACE,"xx:%d to:%o ", xx, pc+2*xx);
}*/
void get_xx(word w)
{
    char bit_7 = (w >> 7) & 1;
	if (bit_7 == 1) {
        xx = w | 0xFF00;
    }
    else {
        xx = w & 0377;  //xx - знаковое, 8 бит
    }
    trace(TRACE, "%o ", pc+2*xx);
	//trace("XX:%d to:%o ", XX, pc+2*XX);
   
}



void do_halt(){
    trace(TRACE, "\n\n---------------- halted ---------------\n");
    reg_print(TRACE);
    flag_print(TRACE);
    trace(TRACE, "THE END!!!\n");
    exit(0);
}

/*void do_mov(){
    if (is_byte_cmd){
        check_sign_bit();   
    }
    else {
        w_write(dd.adr, ss.val);     
    }
    check_NZ(ss.val);
    
}*/
void do_mov(){
    if (b_or_w){
        check_sign_bit();   
    }
    else {
        w_write(dd.adr, ss.val);     
    }
    check_NZ(ss.val);
    
}
  
void do_add(){
    w_write(dd.adr, ss.val + dd.val);
    int res = ss.val;
	res += dd.val;
	check_NZ(ss.val + dd.val);
	check_C(res);
}
void do_inc() {
    w_write(dd.adr, dd.val + 1);
}


void do_sob() {
    //reg[r] --;
    //printf("nn = %o ", nn);
    if (--reg[ri] != 0)
        pc = pc - (2 * nn);
    //printf("%06o ", pc);

} 

void do_clr() {
    if (b_or_w)
		w_write(dd.adr, 0);
	else
		b_write(dd.adr, 0);
    N = 0;
	Z = 1;
	V = 0;
	C = 0;
}

// chỉ dùng để kiểm tra giá trị và thiết lập cờ cho gtri đó, không lm thay đổi nó

void do_cmp() {   //вычисляет разность аргументов ss-dd и по этой разности выставляет все флаги
    w_write(dd.adr, ss.val - dd.val);
    int res = ss.val;
	res -= dd.val;
	check_NZ(ss.val - dd.val);
	check_C(res);   
}
void do_cmpb() {
    b_write(dd.adr, ss.val - dd.val);
    int res = ss.val;
	res -= dd.val;
	check_NZ(ss.val - dd.val);
	check_C(res);
}


void do_br() {      // dung de nhay den dc cần do_br, не изменяет флаги
    pc = pc + 2 * xx;
} 

void do_bcc() // if Carry clear
{
	if(C == 0)
		do_br();
}
void do_bcs() //Branch if Carry Set
{
	if (C == 1)
		do_br();
}

void do_beq() {    //Branch if Equal đay là cuối dòng?
    if(Z == 1)
        do_br();
}
void do_bne() {   //Branch if Not Equal
    if(Z == 0)
        do_br();
}

void do_tst() {
    //unsigned int val = w_read(dd.adr);
    check_NZ(dd.val);
    C = 0;
    //trace(TRACE, "%s %d\n", __FUNCTION__, __LINE__);
    //printf("dd.a = %06o val = %06o b = %d ss.val = %06o", dd.adr, val, b, ss.val);

}

void do_bmi() {   //Branch if Minus
    if(N == 1)
        do_br();
}
void do_bpl() {   //Branch if Plus  
    if(N == 0)
        do_br();
}

void do_jsr()
{
    trace(TRACE, "pc");
	sp -= 2;
	w_write(sp, reg[ri]);
	reg[ri] = pc;
	pc = dd.adr;// + 2; 
}

void do_rts()
{
    // trace("pc = %o\n", pc);
    // trace("sp = %o\n", sp);
    // trace("R%d = %o\n", r, reg[r]);
	//reg[r] = pc;
    // trace("pc = %o\n", pc);
    if (ri == 7){
        trace(TRACE, "pc");
    }
    else {
        trace(TRACE, "R%d", ri);
    }
	pc = reg[ri];
    reg[ri] = w_read(sp);
    // trace("R%d = %o\n", r, reg[r]);
	sp += 2;
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
            trace(TRACE, "R%d ", r);
            break;
        // (R3) mode 1 một thanh ghi, truy cập vào bộ nhớ    
        case 1:   
            res.adr = reg[r];
            if(byte == 0)
                res.val = w_read(res.adr);
            else                
                res.val = b_read(res.adr); //b_read
            //check_op(&res);    
            trace(TRACE, "(R%d) ", r);
            break; 
        // (Rn3)+ ИЛИ #3, мода 2,chế độ địa chỉ tăng tự động автоинкрементный режим адресации    
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
                trace(TRACE, "(R%d)+ ", r);    
            break;
        // @(Rn)+, chế độ 3, chế độ địa chỉ tăng tự động gián tiếp    
        case 3:
            res.adr = reg[r];
            res.adr = w_read(res.adr);
            //res.val = w_read(res.adr);
            if (byte == 0)
                res.val = w_read(res.adr); 
            else
                res.val = b_read(res.adr);
            //check_op(&res);     
            reg[r] += 2;    
            if (r == 7)
                printf("@#%o ", res.adr );
            else
                printf("@(R%d)+ ", r);
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
                trace(TRACE, "-(R%d) ", r);    
            break;
        //Chế độ @-(Rn) 5, chế độ định địa chỉ tự động giảm gián tiếp
        case 5:
            reg[r] -= 2;        
            res.adr = reg[r];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            if (r == 7)
                trace(TRACE, "@#%o ", res.adr );
            else
                trace(TRACE, "@-(R%d) ", r);
            break; 

	    case 6:
		    //trace(TRACE, "pc = %o\n", pc);
            int x = w_read(pc);
            pc += 2;
            //trace(TRACE, "x = %o\n", x); 
            res.adr = reg[ri] + x;
            if (byte == 0)
                res.val = w_read(res.adr);
            else
                res.val = b_read(res.adr);
            if (r != 7) {
                trace(TRACE, "%o(R%d) ", x, r);
            }
            else {
                trace(TRACE, "#%o ", res.adr);
            }
            break;   

        // khi sai thi cac buoc sau ko lm nua    
        default:
            fprintf(stderr, "Mode %o NOT IMPLEMENTED YET!\n", mode);
            exit (1);           
    }

    return res;
}



void check_op(Arg * res) {
    if (b_or_w) {
        res->val = w_read(res->adr);
    }
    else {
        res->val = b_read(res->adr);  
    }  
}
