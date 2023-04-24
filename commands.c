#include "mem.h"
#include "logging.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

Arg ss, dd;
word nn, r;


Command cmd[] = {
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
    {0170000, 0110000, "movb", do_mov, HAS_SS | HAS_DD},
    {0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
    {0177000, 0077000, "sob", do_sob, HAS_NN | HAS_RL},
    {0177000, 0005000, "clr", do_clr, HAS_DD},
    {0000000, 0000000, "nothing", do_nothing, NO_PARAMS}   // Всегда последняя команда
};


void check_sign_bit() {    // проверка знакового бита
    char bit_7 = (ss.val >> 7) & 1;
    //debug("ss.val = %o ", ss.val);
    //debug("bit_7 = %o ", bit_7);
	if (bit_7 == 1) {
        //debug("ss.val | 0xFF00 = %o ", ss.val | 0xFF00);
        w_write(dd.adr, ss.val | 0xFF00);
        //debug("w = %06o\n", w_read(ss.adr));
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

void do_halt(){
    trace(TRACE, "THE END!!!\n");
    reg_print(TRACE);
    exit(0);
}
void do_mov(){
    if (is_byte_cmd){
        check_sign_bit();
        //more_debug("\n---to_val:%o, to_adr:%o, from_val:%o, from_adr:%o---\n", b_read(dd.adr), dd.adr, ss.val, ss.adr);
    }
    else {
        w_write(dd.adr, ss.val);
        //debug("is_word_cmd");
    }
    //trace(TRACE, "ss.adr=%o ss.val=%o dd.adr=%o dd.val=%o\n", ss.adr, ss.val, dd.adr, dd.val);
    //w_write(dd.adr, ss.val);  //vt vao dau & lay tu dau
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




/*
void test_parse_mov()
{
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    Command tcmd = parse_cmd(0010102);
    trace(TRACE, "cmd.name: %s \n", tcmd.name);
    assert(strcmp(tcmd.name, "mov")==0);
    trace(TRACE, " ... OK\n");
}

// тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3
void test_mode0()
{
trace(TRACE, "\n \n");
trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
reg[3] = 12; // dd
reg[5] = 34; // ss
parse_cmd(0010503);

trace(TRACE, "ss: adr=%o val=%d\n", ss.adr, ss.val);
trace(TRACE, "dd: adr=%o val=%d\n", dd.adr, dd.val);
assert(ss.val == 34);
assert(ss.adr == 5);
assert(dd.val == 12);
assert(dd.adr == 3);
trace(TRACE, " ... OK\n");
}

// тест, что mov и мода 0 работают верно в mov R5, R3
void test_mov()
{
trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
reg[3] = 12; // dd
reg[5] = 34; // ss
Command tcmd = parse_cmd(0010503);
tcmd.do_func();
assert(reg[3] == 34);
assert(reg[5] == 34);
trace(TRACE, " ... OK\n");
}

void test_mode1_toreg()
{
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    // setup
    reg[3] = 12;    // dd
    reg[5] = 0200;  // ss
    w_write(0200, 34);
    Command cmd1 = parse_cmd(0011503);
    assert(ss.val == 34);
    assert(ss.adr == 0200);
    assert(dd.val == 12);
    assert(dd.adr == 3);
    cmd1.do_func();
    assert(reg[3] = 34);
    // проверяем, что значение регистра не изменилось
    assert(reg[5] = 0200);
    trace(TRACE, " ... OK\n");
}
*/


