#include "mem.h"

#include <stdio.h>
#include <assert.h>
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

// // test byte read after word write
//     fprintf(stderr, "Test byte read after word write\n");
//     a = 4;
//     w = 0xdeff;
//     b0 = 0xcd;
//     b1 = 0xab;
//     w_write(a, w);
//     bres = b_read(a+1);
//     fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx bres=%02hhx\n", a, b1, b0, bres);
//     assert(b1 == bres);

//     // test byte write after word write
//     fprintf(stderr, "Test byte write after word write\n");
//     a = 6;
//     w = 0xf00d;
//     b0 = 0xef;
//     b1 = 0xbe;
//     w_write(a, w);
//     b_write(a+1, b0);
//     b_write(a+2, b1);
//     wres = w_read(a);
//     bres = b_read(a+1);
//     fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx wres=%04hx\n", a, b1, b0, wres);
//     assert(w == wres && b0 == bres);







// void test_mem(){
//     byte b0= 0x0a;// dinh nghia gtri b0 bang сach положим vao no grti 0x0a
//     // пишем байтб читаем байт(8bit)
//     b_write(2, b0);
//     byte bres = b_read(2);
//     printf("%02hhx = %02hhx\n", b0, bres);//печататься шестнадцатеричные числа шириной два с ведущими нулями шириной
//     assert(b0 == bres);//danh gia bieu thuc va phat hien ra loi
//     // пишем 2 байта читаем 1 слово

//     adr a = 4;
//     byte b1 = 0x0b;
//     b0 = 0x0a;
//     word w = 0x0b0a;
//     b_write(a, b0);
//     b_write(a+1, b1);
//     word wres = w_read(a);
//     printf("ww/br \t %04hx = %02hhx%02hhx\n", wres, b1, b0);
//     assert(w == wres);


// }

