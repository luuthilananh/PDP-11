#include "mem.h"
#include "run.h"
#include "commands.h"
#include "logging.h"
#include "all_tests.h"
#include <string.h>


int main(int argc, const char * argv[])
{
    all_tests();
    trace(TRACE, "------------------------------------------------\n");
    /*set_log_level(MORE_DEBUG);
    all_tests();
    trace(TRACE, "------------------------------------------------\n");
    */

    const char * filename = "tests/pdp11_tests/10_jsr_sum_r5/10_jsr_sum_r5.pdp.o";
    load_file(filename);
    mem_dump(0x200, 0xc);
    trace(TRACE, "%s:%d\n", __FUNCTION__, __LINE__);
    set_ostat();
    run();
    

    return 0;
}