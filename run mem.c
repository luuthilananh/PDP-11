#include <stdlib.h>
#include <assert.h>
typedef unsigned char byte; 
typedef unsigned short int word;
typedef word address;

#define MEMSIZE (64*1024)

// byte thi co the ca dai chi chan va le
byte mem[MEMSIZE];

void b_write (address adr, byte val){
    mem[adr] = b;//vt mem[adr] tg ung vs byte b

}
byte b_read (address adr){
    return mem[adr];
}
void w_write (address adr, word val){
    word w = ((word)mem[a+1]) << 8;
    w = w | mem[a] & 0xFF;
    return w;

}
word w_read (address adr);
void test_mem(){
     byte b0= 0x0a;// dinh nghia gtri b0 bang сach положим vao no grti 0x0a
    // пишем байтб читаем байт(8bit)
    b_write(2, b0);
    byte bres = b_read(2);
    printf("%02hhx = %02hhx\n", b0, bres);//печататься шестнадцатеричные числа шириной два с ведущими нулями шириной
    assert(b0 == bres);//danh gia bieu thuc va phat hien ra loi
    // пишем 2 байта читаем 1 слово

    adress a = 4;
    byte b1 = 0x0b;
    b0 = 0x0a;
    word w = 0x0b0a;
    b_write(a, b0);
    b_write(a+1, b1);
    word wres = w_read(a);
    printf("ww/br \t %04hx = %02hhx%02hhx\n", wres, b1, b0);
    assert(w == wres);

}

// word bat buoc ta ca cac dia chii phai la le 
//word mem[MEMSIZE];

int main()
{
    test_mem()
     return 0;



}