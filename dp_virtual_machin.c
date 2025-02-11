/* PROJECTNAME.c */
#include "dp_virtual_machin.h"

VM *virtualMachine(Program prog, int16 prog_size){

    VM *p;
    Program prog_pointer;
    int16 size;

    assert((prog) && (prog_size));

    size = $2 sizeof(struct s_vm);
    p = (VM*)(malloc($i size));

    if(!p){
        errno = ErrMem;
        return (VM *)0;  //return empty VM
    };

    zero($1 p, size);

    prog_pointer = (Program)(malloc($i prog_size));
    if(!prog_pointer){
        free(p);
        errno = ErrMem;
        return (VM *)0;
    };

    copy($1 prog_pointer, $1 prog, prog_size);

    return p;

}

int8 map_inst(Opcode o){
    int8 n, ret = -1;
    IM *p;

    for(n=sizeOfIM, p=instrmap; n; n--, p++){
        if(p->o == o){
            ret = p->size;
        }
    }
    return ret;
}

Program exampleProgram(){
    int16 size;
    
    Instruction i1, i2;

    size = map_inst(mov);
    i1 = (Instruction)malloc($i size);
    if(!i1){
        errno = ErrMem;
        return (Program)0;
    }
    // i1->o = mov;
    // i1->a[0] = 0x00;
    // i1->a[1] = 0x05;

    size = map_inst(nop);
    i2 = (Instruction)malloc($i size);
    if(!i2){
        errno = ErrMem;
        return (Program)0;
    }
    // i1->o = mov;
    // i1->a[0] = 0x00;
    // i1->a[1] = 0x05;
    Program prog = {i1, i2};
    return prog;
}

int main(int argc, char *argv[]) {
    int8 size;
    Program prog;
    VM *vm;
    size = (map_inst(mov) + map_inst(nop));
    prog = exampleProgram();
    printf("program  = %p\n", prog);

    vm = virtualMachine(prog, size);
    printf("vm       = %p\n", vm);

    return 0;
}