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
     trace(TRACE, "Test byte read/write at even address\n");
    a = 0;
    b0 = 0x0a;
    b_write(a, b0);
    bres = b_read(a);
    trace(TRACE, "a=%06o b0=%02hhx bres=%02hhx\n", a, b0, bres);
    assert(b0 == bres);

    // test byte read/write at odd address
    trace(TRACE, "Test byte read/write at odd address\n");
    a = 1;
    b0 = 0x0a;
    b_write(a, b0);
    bres = b_read(a);
    trace(TRACE, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
    assert(b0 == bres);

    // test word read/write
    trace(TRACE, "Test word read/write\n");
    a = 2;
    w = 0x0b0a;
    w_write(a, w);
    wres = w_read(a);
    trace(TRACE, "a=%06o w=%04hx wres=%04hx\n", a, w, wres);
    assert(w == wres);

    // test word read/write at odd address
    trace(TRACE, "Test word read/write at odd address\n");
    a = 3; 
    w = 0x9abc;
    w_write(a, w);
    wres = w_read(a);
    trace(TRACE, "a=%06o w=%04hx wres=%04hx\n", a, w, wres);
    assert(w == wres);

    // Writing 2 bytes, reading 1 word
    trace(TRACE, "Test write 2 bytes, reading 1 word\n");
    a = 4;
    b1 = 0x0b;
    b0 = 0x0a;
    w = 0x0b0a;
    b_write(a, b0);
    b_write(a+1, b1);
    wres = w_read(a);
    trace(TRACE,"bw/br \t %04hx = %02hhx%02hhx\n", wres, b1, b0);
    assert(w == wres);

    // Write one word read two bytes
    trace(TRACE, "Test write one word read two bytes\n");
    a = 4;
    b1 = 0x0b;
    b0 = 0x0a;
    w = 0x0b0a;
    w_write(a, w);
    byte b0res = b_read(a);
    byte b1res = b_read(a+1);
    trace(TRACE,"ww/br \t %04hx = %02hhx%02hhx\n", w, b1res, b0res);
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

void test_mode1()
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

void test_mode2()
{
    // INC (R3)+
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    w_write(1000, 5);
    reg[3] = 1000;    // dd
    Command tcmd = parse_cmd(0005223);
    tcmd.do_func();
    assert(w_read(reg[3] - 2) == 6);
    assert(reg[3] == 1002);
    trace(TRACE, "\n ... OK\n");
}

void test_mode3()
{
    // MOV @R3+, R1
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    w_write(1010, 776);
    w_write(776, 5);
    reg[3] = 1010;    // dd

    Command tcmd = parse_cmd(0013301);
    tcmd.do_func();
    assert(w_read(reg[3] - 2) == 776);
    assert(reg[1] == 5);
    trace(TRACE,"\n ... OK\n");
}

void test_mode4()
{
    // INC -(R3)
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    w_write(1000, 5);
    reg[3] = 1002;    // dd

    Command tcmd = parse_cmd(0005243);
    tcmd.do_func();
    assert(w_read(reg[3]) == 6);
    trace(TRACE,"\n ... OK\n");
}

void test_mode5()
{
    // INC @-(R3)
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    w_write(1012, 776);
    w_write(776, 5);
    reg[3] = 1014;    // dd

    Command tcmd = parse_cmd(0005253);
    tcmd.do_func();
    assert(w_read(w_read(reg[3])) == 6);
    trace(TRACE,"\n ... OK\n");
}

void test_mode61()
{
    // MOV 4(R3), R1
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    w_write(01006, 04);
    w_write(0204, 15);
    reg[3] = 0200;
    //reg[1] = 05;
    pc = 01006;
    trace(DEBUG, "R3 = %o \n", reg[3]);
    trace(DEBUG, "mem[1006] = %o \n", w_read(01006));
    trace(DEBUG, "mem[1010] = %o \n", w_read(01010));   
    Command tcmd = parse_cmd(0016301);
    tcmd.do_func();
    trace(DEBUG, "mem[%o] = %o \n",reg[3] + 4,  w_read(reg[3] + 4)); 
    assert(reg[1] == 15);
    trace(TRACE,"\n ... OK\n");
}

void test_mode6()
{
	// setup
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
	reg[5] = 34;  // ss
	reg[3] = 0300;	// dd
	pc = 01002;
	w_write(01002, 02);
	w_write(0302, 12);

	Command test_cmd = parse_cmd(0010563);
    trace(TRACE, "dd.val = %d", dd.val);
	assert(dd.val == 12);
	assert(dd.adr == 0302);
	assert(ss.val == 34);
	assert(ss.adr == 5);

	test_cmd.do_func();

	assert(w_read(0302) == 34); //проверяем прошло ли успешно копирование

	assert(reg[3] == 0300);
	assert(reg[5] == 34);
    trace(TRACE, "\n ... OK\n");
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

void test_clr(){
    trace(TRACE, "%s , %d\n",__FUNCTION__, __LINE__);
    reg[3] = 12;    // dd
    trace(TRACE, "reg[3] = %o\n", reg[3]);
    Command tcmd = parse_cmd(0005003);
    tcmd.do_func();
    assert(reg[3] == 0);
    trace(TRACE, "\n ... OK\n");
}

void all_tests()
{
	test_mem();
	test_parse_mov();
    test_mode0();
    test_mode1();
    test_mode2();
    test_mode3();
    test_mode4();
    test_mode5();
    test_mode61();
    test_mode6();
    test_mov();
    test_clr();
    
}
