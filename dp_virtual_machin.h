/* dp_virtual_machine.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <dputils.h>

#define NoErr       0x00         // 00 00
#define SysHlt      0x01         // 00 01
#define ErrMem      0x02         // 00 10
#define ErrSegv     0x04         // 01 00

typedef unsigned char Errorcode;
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

#define $ax ->c.r.ax
#define $bx ->c.r.bx
#define $cx ->c.r.cx
#define $dx ->c.r.dx
#define $ip ->c.r.ip
#define $sp ->c.r.sp
#define $bp ->c.r.bp


#define segfault(x)     error((x), ErrSegv)

// this is 16 bit vm

typedef unsigned short int reg;

struct s_registers{
    reg ax;
    reg bx;
    reg cx; 
    reg dx;
    reg ip;
    reg sp;
    reg bp;
};

enum registers{
    ax = 0x00,
    bx = 0x01,
    cx = 0x02,
    dx = 0x03,
    ip = 0x04,
    sp = 0x05,
    bp = 0x06,
};

typedef struct s_registers Registers;

struct s_cpu{
    Registers r;
};

typedef struct s_cpu CPU;

/*
    mov ax, 0x05; 
    (mov = 0x08 = 0000 1000, ax = 0x00 = 0000 0000,
     base opcode = 0x08 and final opcode = 0x08 + 0x00 = 0x08
     0x05 is oprend (size 2 byte) = 0000 0101 0000 0000 (littel endian)
     so final instraction is 0x08 0x05 0x00
    )
*/
enum s_opcode{
    mov = 0x08,
    nop = 0x10,
    hlt = 0x18,
    add = 0x20,
    push = 0x28,
    pop = 0x30,
    jmp = 0x38,
};

typedef int8 Opcode;

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

typedef int8 Memory[65535];
typedef int8 Program;

struct s_vm
{
    CPU c;
    Memory m;
    int16 b; // break line
};
/*
there is .text and .data section

.text (+r -w +e)

###########
#####
---------------------------  break line

*****
************
.data (+r +w -e)
*/

typedef struct s_vm VM;

static IM instrmap[] = {
    { mov, 0x03},
    { nop, 0x01},
    { hlt, 0x01},
    { add, 0x03},
    { push, 0x03},
    { pop, 0x01},
    { jmp, 0x03},
};

#define sizeOfIM ((sizeof(instrmap)) / (sizeof(instrmap[0])))

void _add(VM*, int8, Args, Args);
void _mov(VM*, int8, Args, Args);
void _push(VM*, Args, Args);
void _pop(VM*, int8);
void _jmp(VM*, Args, Args);
Instruction *copy_instruction(Program*, int16);
 Program *exampleProgram(VM*);
void execute(VM*);
void error(VM*, Errorcode);
int8 map_inst(Opcode);
VM *virtualMachine(void);

int main(int,char**);