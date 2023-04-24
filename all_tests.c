#include "mem.h"
#include "commands.h"
#include "run.h"
#include "logging.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>



void test_mem()
{
    adr a;
    byte b0, b1, bres;
    word w, wres;

    // test byte read/write
    fprintf(stderr, "Test byte read/write at even address\n");
    a = 0;
    b0 = 0x0a;
    b_write(a, b0);
    bres = b_read(a);
    fprintf(stderr, "a=%06o b0=%02hhx bres=%02hhx\n", a, b0, bres);
    assert(b0 == bres);

    // test byte read/write at odd address
    fprintf(stderr, "Test byte read/write at odd address\n");
    a = 1;
    b0 = 0x0a;
    b_write(a, b0);
    bres = b_read(a);
    fprintf(stderr, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
    assert(b0 == bres);

    // test word read/write
    fprintf(stderr, "Test word read/write\n");
    a = 2;
    w = 0x0b0a;
    w_write(a, w);
    wres = w_read(a);
    fprintf(stderr, "a=%06o w=%04hx wres=%04hx\n", a, w, wres);
    assert(w == wres);

    // test word read/write at odd address
    fprintf(stderr, "Test word read/write at odd address\n");
    a = 3; 
    w = 0x9abc;
    w_write(a, w);
    wres = w_read(a);
    fprintf(stderr, "a=%06o w=%04hx wres=%04hx\n", a, w, wres);
    assert(w == wres);

    // Writing 2 bytes, reading 1 word
    a = 4;
    b1 = 0x0b;
    b0 = 0x0a;
    w = 0x0b0a;
    b_write(a, b0);
    b_write(a+1, b1);
    wres = w_read(a);
    printf("bw/br \t %04hx = %02hhx%02hhx\n", wres, b1, b0);
    assert(w == wres);

    // Write one word read two bytes
    a = 4;
    b1 = 0x0b;
    b0 = 0x0a;
    w = 0x0b0a;
    w_write(a, w);
    byte b0res = b_read(a);
    byte b1res = b_read(a+1);
    printf("ww/br \t %04hx = %02hhx%02hhx\n", w, b1res, b0res);
    assert(b0 == b0res);
    assert(b1 == b1res);

    
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
