/* PROJECTNAME.c */
#include "dp_virtual_machin.h"

VM *virtualMachine(){

    VM *virtual_machine;
    // Program *prog_pointer;
    int16 size;

    size = $2 sizeof(struct s_vm);
    virtual_machine = (VM*)(malloc($i size));

    if(!virtual_machine){
        errno = ErrMem;
        return (VM *)0;  //return empty VM
    };

    zero($1 virtual_machine, size);
    
    /*
    prog_pointer = (Program *)(malloc($i prog_size));
    if(!prog_pointer){
        free(p);
        errno = ErrMem;
        return (VM *)0;
    };

    copy(prog_pointer, prog, prog_size);
    */

    return virtual_machine;

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

// Program exampleProgram(){
//     int16 size;
    
//     Instruction i1, i2;

//     size = map_inst(mov);
//     i1 = (Instruction)malloc($i size);
//     if(!i1){
//         errno = ErrMem;
//         return (Program)0;
//     }
//     // i1->o = mov;
//     // i1->a[0] = 0x00;
//     // i1->a[1] = 0x05;

//     size = map_inst(nop);
//     i2 = (Instruction)malloc($i size);
//     if(!i2){
//         errno = ErrMem;
//         return (Program)0;
//     }
//     // i1->o = mov;
//     // i1->a[0] = 0x00;
//     // i1->a[1] = 0x05;
//     Program prog = {i1, i2};
//     return prog;
// }

Program *exampleProgram(VM *vm){
    Program *prog;
    Instruction *i1, *i2;
    Args *a1;
    int16 s1, s2, sa1;

    s1 = map_inst(mov);
    s2 = map_inst(nop);

    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc($i s2);
    assert(i1 && i2);
    zero($1 i1, s1);
    zero($1 i2, s2);

    i1->o = mov;
    sa1 = s1 - 1;
    if(sa1){
        a1 = (Args *)malloc($i sa1);
        zero(a1, sa1);
        *a1 = 0x00;
        *(a1+1) = 0x05;
    }
    i2->o = nop;

    // load instriction 1 in memory
    // program_size = (s1 + s2);
    prog = vm->m;
    copy($1 prog, $1 i1, 1);
    prog++;
    if(sa1){
        copy($1 prog, $1 a1, sa1);
        prog+=sa1;
        free(a1);
    }

    //load inst 2 in memory
    copy($1 prog, $1 i1, 1);
    free(i1);
    free(i2);

    return vm->m;

    
}

int main(int argc, char *argv[]) {
    Program *prog;
    VM *vm;
    
    vm = virtualMachine();
    printf("vm       = %p\n", vm);

    prog = exampleProgram(vm);
    printf("program  = %p\n", prog);

    return 0;
}