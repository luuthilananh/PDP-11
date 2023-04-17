#include "mem.h"
#include "logging.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void run(){
    pc = 01000;
    fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__);
    trace(TRACE, "%s:%d\n", __FUNCTION__, __LINE__);
    while(1){
        word w = w_read(pc); 
        trace(TRACE, "%06o %06o ", pc, w);
        pc +=2;

        for (unsigned long int i = 0; ; i++){
            if ((w & cmd[i].mask) == cmd[i].opcode){
                trace(TRACE, "%s ", cmd[i].name);
                ss = get_mr(w >> 6);
                dd = get_mr(w);
    trace(TRACE, "ss.adr=%o ss.val=%o dd.adr=%o dd.adr=%o\n", ss.adr, ss.val, dd.adr, dd.val);
                cmd[i].do_func();
                trace(TRACE, "\n");
                reg_print(TRACE);

                break;

            }
        }
    }

}

