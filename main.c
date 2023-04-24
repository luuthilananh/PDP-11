
#include "mem.h"
#include "run.h"
#include "commands.h"
void test_mem();


int main(int argc, const char * argv)
{
   
    const char * filename = "tests/pdp11_tests/03_arr0/03_arr0.pdp.o";
    
    load_file(filename);
    mem_dump(0x200, 0xc);
    fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__);
    
    run();
    /*
    test_mem();
    test_parse_mov();
    test_mode0();
    test_mov();
    test_mode1_toreg();
    */
    return 0;
}