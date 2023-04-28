#pragma once

/*#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2
#define HAS_NN 4
#define HAS_XX 8
#define HAS_RL 16
#define HAS_RR 32
*/
enum params {   
    NO_PARAMS,
	HAS_DD,
	HAS_SS,
	HAS_RL = 1<<2,
	HAS_NN = 1<<3,
	HAS_XX = 1<<4,
	HAS_RE = 1<<5  // has r in the end
};

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
//extern int nn, ri;
extern word nn, ri;
extern int b_or_w;
//extern char is_byte_cmd;
extern word reg[REGSIZE];
extern byte mem[MEMSIZE];
extern signed char xx; // do_br nhảy địa chỉ đến bất kì đâu nhớ là nhảy theo từ
extern char Z, N, V, C;

void check_sign_bit();
void check_NZ(word w);
void check_C(int x);
char get_nn (word w);
void get_xx(word w);
//char get_xx (word w);
int get_r (word w);

void do_halt();
void do_mov();
void do_add();
void do_inc();
void do_sob();
void do_clr();
void do_cmp();
void do_cmpb();
void do_br();
void do_bcc();
void do_bcs();
void do_beq();
void do_bne();
void do_tst();
void do_bmi();
void do_bpl();
void do_jsr();
void do_rts();
void do_jmp();
void do_nothing();
Arg get_mr(int byte, word w);
void check_op(Arg * res);








