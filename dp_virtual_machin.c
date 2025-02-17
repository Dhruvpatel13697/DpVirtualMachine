/* PROJECTNAME.c */
#include "dp_virtual_machin.h"

void _mov(VM * vm, Opcode o, Args a1, Args a2){
    vm $ax = (reg)a1;

    return;
}

void execinstr(VM *vm, Instruction *i){

}

void execute(VM *vm){
     Instruction *ip;
     Program *pp;
     int16 size = 0;

     assert(vm && *vm->m);
     pp = vm->m;


     while(((*pp != (Opcode)hlt) & (pp <= (vm->m + vm->b)))){
        switch (*pp)
        {
        case (Opcode)mov:
            size = map_inst(mov);
            ip = (Instruction *)malloc($i size);
            zero($1 ip, size);
            copy($1 ip, $1 pp, size);
            _mov(vm, ip->o, ip->a[0], ip->a[1]);
            break;

        case (Opcode)nop:
            size = map_inst(nop);
            break;
        
        default:
            segfault(vm);
            break;
        }

        // ip = (Instruction *)pp;
        // size = map_inst(ip->o);
        // execinstr(vm, ip);

        vm $ip += size;  
        pp += size;
     }
    if(pp > (vm->m + vm->b)){
        segfault(vm);
    }

}

void error(VM *vm, Errorcode e){

    int8 exitcode = -1;

    if(vm){
        free(vm);
    }

    switch (e)
    {
    case ErrSegv:
        fprintf(stderr, "%s\n", "VM segmentfault fault");
        break;
    case SysHlt:
        fprintf(stderr, "%s\n", "system halted");
        exitcode = 0;
        break;
    default:
        break;
    }

    exit($i exitcode);
}

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

Program *exampleProgram(VM *vm){
    Program *prog;
    Instruction *i1, *i2 , *i3;
    Args a1;
    int16 s1, s2, s3, sa1;

    s1 = map_inst(mov);
    s2 = map_inst(nop);
    s3 = map_inst(hlt);


    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc($i s2);
    i3 = (Instruction *)malloc($i s3);
    assert(i1 && i2);
    zero($1 i1, s1);
    zero($1 i2, s2);
    zero($1 i3, s3);

    i1->o = mov;
    sa1 = s1 - 1;
    a1 = 0x0005;
   
    i2->o = nop;
    i3->o = hlt;

    // load instriction 1 in memory
    // program_size = (s1 + s2);
    prog = vm->m;
    copy($1 prog, $1 i1, 1);
    prog++;
    copy($1 prog, &a1, sa1);
    prog+=sa1;
   

    //load inst 2 in memory
    copy($1 prog, $1 i2, 1);
    prog++;
    copy($1 prog, $1 i3, 1);
    vm->b = (s1 + s2 + s3);
    vm $ip = (reg)vm->m;
    vm $sp = (reg)-1;
    free(i1);
    free(i2);
    free(i3);

    return (Program *)(&vm->m);

    
}

int main(int argc, char *argv[]) {
    Program *prog;
    VM *vm;
    
    vm = virtualMachine();
    printf("vm       = %p (size=%ld)\n", vm, sizeof(VM));

    prog = exampleProgram(vm);
    printf("program  = %p\n", prog);
    printf("ax       = %.04hx\n", $i vm $ax);

    printhex($1 prog, (map_inst(mov) + map_inst(nop) + map_inst(hlt)), ' ');
    execute(vm);
    printf("ax after = %.04hx\n", $i vm $ax);

    return 0;
}