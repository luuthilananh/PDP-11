#include "mem.h"
#include "run.h"
#include "commands.h"
#include "logging.h"
#include "all_tests.h"
#include <string.h>


int main(int argc, const char * argv[])
{
    
    //set_log_level(MORE_DEBUG);
    
    all_tests();
    //memset(reg, 0, sizeof(reg));
    trace(TRACE, "------------------------------------------------\n");

    const char * filename = "tests/pdp11_tests/08_hello/08_hello.pdp.o";

    load_file(filename);
    mem_dump(0x200, 0xc);
    fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__);
    set_ostat();
    //fprintf(stderr, "%s %d\n", __FUNCTION__, __LINE__);
    run();
    

    return 0;
}