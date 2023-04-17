
#include "mem.h"
#include "run.h"
#include "commands.h"
void test_mem();


int main(int argc, const char * argv)
{
   

    // test_mem();
    
    
    // return 0;

    const char * filename = "tests/pdp11_tests/01_sum/01_sum.pdp.o";
    
    load_file(filename);
    mem_dump(0x200, 0xc);
    fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__);
    
    //run();
    test_parse_mov();
    test_mode0();
    test_mov();

    return 0;
}