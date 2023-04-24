#include "mem.h"
#include "logging.h"
#include "run.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int b;
char is_byte_cmd;

Command parse_cmd(word w) { 
    //word w = w_read(pc);
    for (unsigned long int i = 0; ; i++){
        Command cmdd = cmd[i];
        //trace("%s \n", cmd.name);
        if ((w & cmdd.mask) == cmdd.opcode) {
            trace(TRACE, "%s       ", cmdd.name);

            if (cmdd.params & HAS_SS){
                ss = get_mr(b, w >> 6); 
            }
            if (cmdd.params & HAS_DD){
                dd = get_mr(b, w); 
            }
            if (cmdd.params & HAS_RL)
                r = get_r(w >> 6) & 7;
            if (cmdd.params & HAS_RR)
                r = get_r(w);
            if ((cmdd.params & HAS_XX)) {
                xx = get_xx(w);
                if (xx >> 7)
                    xx = xx - 0400;
                printf("%06o", pc + 2 * xx);
            }
            if (cmdd.params & HAS_NN){   
                nn = get_nn(w);
                printf("%o ", pc - 2 * nn);
            } 
            return cmdd;   
            /*
            if (cmdd.params & HAS_RL){
                r = (w>>6) & 07;
                //trace("r = %o", r);
            }
            if (cmdd.params & HAS_NN){
                nn = w & 077;
                //trace("nn = %o", nn);
                //prn();
            }
            */
            
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
        //trace(TRACE,  "is_b_cmd = %d\n", is_byte_cmd);
        trace(TRACE, "%06o %06o: ", pc, w);
        pc += 2;
        Command cmd = parse_cmd(w);
        cmd.do_func();
        trace(TRACE, "\n");
        flag_print(TRACE);
        reg_print(TRACE);
        trace(TRACE, "\n");
    }
}

