#include "mem.h"
#include "logging.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

Arg ss, dd;

void do_halt(){
    trace(TRACE, "THE END!!!\n");
    reg_print(TRACE);
    exit(0);
}
void do_mov(){
    trace(TRACE, "ss.adr=%o ss.val=%o dd.adr=%o dd.val=%o\n", ss.adr, ss.val, dd.adr, dd.val);
    w_write(dd.adr, ss.val);  //vt vao dau & lay tu dau

}
void do_add(){
    w_write(dd.adr, ss.val + dd.val);
}
void do_nothing(){
    trace(ERROR, "Unknown command\n");
    exit(1);
}



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


Command parse_cmd(word w){
    for (unsigned long int i = 0; ; i++){
        Command cmdd = cmd[i];
        trace(TRACE, "i=%d %s \n", i, cmdd.name);
        if((w & cmdd.mask) == cmdd.opcode){
            trace(TRACE, "%s ", cmdd.name);
            if (cmdd.params & HAS_SS){
                ss = get_mr(w >> 6);
            }
            if (cmdd.params & HAS_DD){
                dd = get_mr(w);
            }
            return cmdd;

        }
    }
}
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


Command cmd[] = {
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
    {0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
    {0000000, 0000000, "nothing", do_nothing, NO_PARAMS}   // Всегда последняя команда
};

