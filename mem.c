#include "mem.h"
#include "commands.h"
#include "logging.h"
#include <assert.h>

word reg[REGSIZE];
byte mem[MEMSIZE];
static int log_level;


byte b_read(adr a) {
    if (a < 8) {
        return reg[a];
    } else {
        return mem[a];
    }
}

void b_write(adr a, byte val) {
    if (a < 8) {
        if ((val >> 7) & 1) {
            reg[a] = (val & 0xFF) | 0xFF00;
        } else {
            reg[a] = val & 0xFF;
        }
        return;
    } 
    if(a == odata)
		printf("%c", val);
    mem[a] = val;
    
}
/*void w_write(adr a, word val) {
    if (a < 8) {
        reg[a] = val;
    } 
    if(a == odata)
		trace(TRACE, "%c", val);
    mem[a] = val & MASK;
    mem[a+1] = val >> 8 & MASK;
    
}*/
word w_read(adr a) {
    word w;
    if (a < 8) {
        w = (reg[a + 1] << 8) | reg[a];
    } else {
        w = (mem[a + 1] << 8) | mem[a];
    }
    return w;
}


void w_write(adr a, word val)
{
	// 0 .. 7 - registers !!!
	if (a < 8) {
		reg[a] = val;
		return;
	}
	assert(a % 2 == 0);
    //trace(TRACE, "a = %o, val = %o\n", a, val);
    //trace(TRACE, "mem[a] = %o,\n", w_read(a));
	mem[a] = val;
	mem[a + 1] = val >> 8;
}

void set_ostat()
{
	//b_write(ostat, (1<<7));
	w_write(ostat, 0xFF);
}

void load_file(const char * filename) {
    FILE * f = fopen(filename, "r");
    adr a, n;
    byte x;
    if (f == NULL)
    {
        perror(filename);
        exit(0);
    }
    while (fscanf(f, "%hx%hx", &a, &n) == 2)
    {
        for (adr i = a; i < a + n; i++) {
            fscanf(f, "%hhx", &x);
            b_write(i, x);
        }
    }
    fclose(f);
}

void mem_dump(adr start, word n) {
    for (adr i = start; i < start + n; i+=2) {
        word w = w_read(i);
        //printf("%04hx\n", w);
        printf("%06ho : %06ho\n", i, w);
    }
}
void reg_print(int trace_level)
{
    for (int i = 0; i < REGSIZE-2; i++)
        trace(trace_level, "r%d:%06o ", i, reg[i]);
    trace(trace_level,"sp:%06o pc:%06o ", sp, pc);    
    trace(trace_level, "\n");
}
void flag_print(int trace_level)
{
    
    trace(trace_level, "%c", N ? 'N': '-');
    trace(trace_level, "%c", Z ? 'Z': '-');
    trace(trace_level, "%c", V ? 'V': '-');
    trace(trace_level, "%c", C ? 'C': '-');
    trace(trace_level, " ");
}

/*int set_log_level(int level)
{
	int old_log_level = log_level;
	log_level = level;
	return old_log_level;
}
*/