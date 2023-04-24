#pragma once

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2
#define HAS_NN 4
#define HAS_XX 8
#define HAS_RL 16
#define HAS_RR 32

typedef struct 
{
    word mask;
    word opcode;
    char * name;
    void (*do_func)();
    char params;
} Command;


typedef struct {
    word val;  //значение аргумента
    adr adr;  //адрес аргумента
} Arg;

extern Arg ss, dd;
extern Command cmd[];
extern word nn, r;
extern int b;
extern char is_byte_cmd;
extern word reg[REGSIZE];
extern byte mem[REGSIZE];
extern signed char xx; // do_br nhảy địa chỉ đến bất kì đâu nhớ là nhảy theo từ
extern char Z, N, V, C;

Arg get_mr(int byte, word w);
char get_nn (word w);
char get_xx (word w);
int get_r (word w);
void set_NZ(word w);
void set_C(int x);

void do_halt();
void do_mov();
void do_add();
void do_sob();
void do_clr();
void do_tst();
void do_tstb();
void do_cmp();
void do_cmpb();
void do_br();
void do_beq();
void do_bne();
void do_bmi();
void do_bpl();
void do_jmp();
void do_nothing();



void test_parse_mov();
void test_mode0();
void test_mov();
void test_mode1_toreg();






 