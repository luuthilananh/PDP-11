#include "mem.h"
#include "logging.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int b;
char is_byte_cmd;
// word reg[REGSIZE];
// byte mem[REGSIZE];

/*Command parse_cmd(word w){
    for (unsigned long int i = 0; ; i++){
        Command cmdd = cmd[i];
        trace(TRACE, "i=%d %s \n", i, cmdd.name);
        if((w & cmdd.mask) == cmdd.opcode){
            trace(TRACE, "%s ", cmdd.name);
            if (cmdd.params & HAS_SS){
                ss = get_mr(b, w >> 6);
            }
            if (cmdd.params & HAS_DD){
                dd = get_mr(b, w);
            }
            return cmdd;

        }
    }
}

void run(){
    pc = 01000;
    //fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__);
    trace(TRACE, "%s:%d\n", __FUNCTION__, __LINE__);
    while(1){
        word w = w_read(pc); 
        trace(TRACE, "%06o %06o ", pc, w);
        pc +=2;

        for (unsigned long int i = 0; ; i++){
            if ((w & cmd[i].mask) == cmd[i].opcode){
                trace(TRACE, "%s ", cmd[i].name);
                ss = get_mr(b, w >> 6);
                dd = get_mr(b, w);
    trace(TRACE, "ss.adr=%o ss.val=%o dd.adr=%o dd.val=%o\n", ss.adr, ss.val, dd.adr, dd.val);
                cmd[i].do_func();
                trace(TRACE, "\n");
                reg_print(TRACE);

                break;

            }
        }
        // Command com = parse_cmd(w);
        // com.do_func;
        //reg_print(TRACE);

    }

}
*/

Command parse_cmd(word w) { 
    //word w = w_read(pc);
    for (unsigned long int i = 0; ; i++){
        Command cmdd = cmd[i];
        //trace("%s \n", cmd.name);
        if ((w & cmdd.mask) == cmdd.opcode) {
            trace(TRACE, "%s       ", cmdd.name);
            //prn();
            if (cmdd.params & HAS_SS){
                ss = get_mr(b, w >> 6); 
            }
            if (cmdd.params & HAS_DD){
                dd = get_mr(b, w); 
            }
            if (cmdd.params & HAS_RL){
                r = (w>>6) & 07;
                //trace("r = %o", r);
            }
            if (cmdd.params & HAS_NN){
                nn = w & 077;
                //trace("nn = %o", nn);
                //prn();
            }
            return cmdd;
        }
            //trace("%s \n", cmd[i].name);
    } 
    //return cmd[0]; 
}

void run() {

    trace(TRACE, "---------------- running --------------\n");
    pc = 01000;
    while(1) {
        word w = w_read(pc);
        is_byte_cmd = (w >> 15) & 1;
        b = is_byte_cmd;
        trace(TRACE,  "is_b_cmd = %d\n", is_byte_cmd);
        trace(TRACE, "%06o %06o: ", pc, w);
        pc += 2;
        Command cmd = parse_cmd(w);
        cmd.do_func();
        trace(TRACE, "\n");
        reg_print(TRACE);
        trace(TRACE, "\n");
    }
}

