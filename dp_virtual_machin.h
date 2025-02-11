/* PROJECTNAME.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <dputils.h>

#define ErrMem     0x01

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)

// this is 16 bit vm

typedef unsigned short int reg;

struct s_registers{
    reg ax;
    reg bx;
    reg cx; 
    reg dx;
    reg ip;
    reg sp;
};

typedef struct s_registers Registers;

struct s_cpu{
    Registers r;
};

typedef struct s_cpu CPU;

/*
    mov ax, 0x05; ( 0x01 or 0x02
                    0000 0011 => first 1 for mov and second 1 for ax reg
                    0000 0000 0000 0000 => these 16 bit use for args ex. 0x05
                    )
*/
enum s_opcode{
    mov = 0x01,
    nop = 0x02
};

typedef enum s_opcode Opcode;

struct s_instrmap{
    Opcode o;
    int8 size;
};

typedef struct s_instrmap IM;

typedef int8 Args;

struct s_instruction{
    Opcode o;
    Args a[]; // 0 - 2 byte
};

typedef struct s_instruction Instruction;


typedef int8 stack[((unsigned int)(-1))];
typedef Instruction Program;

struct s_vm
{
    CPU c;
    stack s;
    Program *p;
};

typedef struct s_vm VM;

static IM instrmap[] = {
    { mov, 0x03},
    { nop, 0x01}
};

VM *virtualMachine(Program*, int16);

int main(int,char**);