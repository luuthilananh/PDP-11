#include "mem.h"
#include "logging.h"
#include "run.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Command parse_cmd(word w) { 
    //word w = w_read(pc);
    for (unsigned long int i = 0; ; i++){
        Command cmdd = cmd[i];
        //trace(TRACE, "%s \n", cmd.name);
        if ((w & cmdd.mask) == cmdd.opcode) {
            trace(TRACE, "%s       ", cmdd.name);

            if (cmdd.params & HAS_SS){
                ss = get_mr(b_or_w, w >> 6); 
            }
            if (cmdd.params & HAS_DD){
                dd = get_mr(b_or_w, w); 
            }
            if (cmdd.params & HAS_RL)
                ri = get_r(w >> 6) & 7;
            if (cmdd.params & HAS_RE)
                ri = get_r(w);
            if (cmdd.params & HAS_NN){   
                nn = get_nn(w);
            } 
            if (cmdd.params & HAS_XX){
                get_xx(w);
                
            }
            
            return cmdd;   
            
            
        }
    } 
   
   }

void run() {

    trace(TRACE, "---------------- running --------------\n");
    pc = 01000;
    while(1) {
        word w = w_read(pc);
        b_or_w = (w >> 15) & 1;
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

