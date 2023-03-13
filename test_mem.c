#include <stdio.h>
#include <assert.h>
typedef unsigned char byte; 
typedef unsigned short int word;
typedef word address;

#define MEMSIZE (64*1024)

// I byte thi co the ca dai chi chan va le
word mem[MEMSIZE];

void b_write (address a, byte b){
    mem[a] = b;//vt mem[a] tg ung vs byte b

}
byte b_read (address adr){
    byte b;
    if (adr % 2 == 0)
        b= mem[adr] & 0xFF; // b=(byte)mem[adr]
    else{
        word w = mem[adr-1];
        b = w >> 8 & 0xFF;// только беру 1 байт
    }    

    return mem[adr];
}
void w_write (address adr, word w){
    assert(adr % 2 == 0);
    mem[adr] = w;
}
word w_read (address a){
    assert(a % 2 == 0);
    word w = ((word)mem[a+1]) << 8;
    w = w | mem[a] & 0xFF;
    return w;
}
void test_mem(){
    byte b0= 0x0a;// dinh nghia gtri b0 bang сach положим vao no grti 0x0a
    // пишем байтб читаем байт(8bit)
    b_write(2, b0);
    byte bres = b_read(2);
    printf("%02hhx = %02hhx\n", b0, bres);//печататься шестнадцатеричные числа шириной два с ведущими нулями шириной
    assert(b0 == bres);//danh gia bieu thuc va phat hien ra loi
    // пишем 2 байта читаем 1 слово

    address a = 4;
    byte b1 = 0x0b;
    b0 = 0x0a;
    word w = 0x0b0a;
    b_write(a, b0);
    b_write(a+1, b1);
    word wres = w_read(a);
    printf("ww/br \t %04hx = %02hhx%02hhx\n", wres, b1, b0);
    assert(w == wres);

    // // пишем слово 2 байта читаем 2 байта
    // address a =4;
    // byte b1 = 0x0b;
    // byte b0 = 0x0a;
    // word w = 0x0b0a;
    // w_write(a, w);
    // word b0res = b_read(a);
    // word b1res = b_read(a+1);
    // printf("ww/br \t %04hx = %02hhx%02hhx\n", w, b1res, b0res);
    // assert(b0 == b0res);
    // assert(b1 == b1res);

}

// II word bat buoc ta ca cac dia chi phai la le 
// word mem[MEMSIZE];
// void test_mem2{
//     address a =4;
//     byte b1 = 0x0b;
//     byte b0 = 0x0a;
//     word w = 0x0b0a;
//     w_write(a, w);
//     word b0res = b_read(a);
//     word b1res = b_read(a+1);
//     printf("ww/br \t %04hx = %02hhx%02hhx\n", w, b1res, b0res);
//     assert(b0 == b0res);
//     assert(b1 == b1res);



// }


int main()
{
    test_mem();
     return 0;



}