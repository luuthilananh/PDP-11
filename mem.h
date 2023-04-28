#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#define MASK 0xff
#define MEMSIZE (64*1024)
#define pc reg[7]
#define sp reg[6]
#define REGSIZE 8
#define ostat 0177564
#define odata 0177566



typedef unsigned char byte; //8 bit
typedef unsigned short int word;  //16 bit
typedef word adr;           //16 bit


byte b_read(adr a);
void b_write(adr a, byte val);
void w_write(adr a, word val);
word w_read(adr a);
void set_ostat();
void load_file(const char * filename);
void mem_dump(adr start, word n);
void reg_print(int trace_level);
void flag_print(int trace_level);
