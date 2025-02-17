/* PROJECTNAME.c */
#include "dp_virtual_machin.h"

void _mov(VM * vm, int8 dest_reg, Args a1, Args a2){
    reg r = ((a2 << 8) + a1);
    switch (dest_reg)
    {
    case ax:
        vm $ax = r;
        break;

    case bx:
        vm $bx = r;
        break;

    case cx:
        vm $cx = r;
        break;

    case dx:
        vm $dx = r;
        break;
    
    default:
        break;
    }

    return;
}


void _add(VM * vm, int8 dest_reg, Args a1, Args a2){
    reg r = ((a2 << 8) + a1);
    switch (dest_reg)
    {
    case ax:
        vm $ax += r;
        break;

    case bx:
        vm $bx += r;
        break;

    case cx:
        vm $cx += r;
        break;

    case dx:
        vm $dx += r;
        break;
    
    default:
        break;
    }

    return;
}


void execute(VM *vm){
     Instruction *ip;
     Program *pp;
     int16 size = 0;
     Opcode opcode;
     int8 dest_reg;

     assert(vm && *vm->m);
     pp = vm->m;


     do{
        opcode = *pp & (0xf8);
        switch (opcode)
        {
        case (Opcode)mov:
            size = map_inst(mov);
            ip = (Instruction *)malloc($i size);
            zero($1 ip, size);
            copy($1 ip, $1 pp, size);
            dest_reg = *pp & 0x07;
            _mov(vm, dest_reg, ip->a[0], ip->a[1]);
            break;
        
        case (Opcode)add:
            size = map_inst(add);
            ip = (Instruction *)malloc($i size);
            zero($1 ip, size);
            copy($1 ip, $1 pp, size);
            dest_reg = *pp & 0x07;
            _add(vm, dest_reg, ip->a[0], ip->a[1]);
            break;

        case (Opcode)nop:
            size = map_inst(nop);
            break;

        case (Opcode)hlt:
            error(vm, SysHlt);
            break;
        
        default:
            segfault(vm);
            break;
        }

        vm $ip += size;  
        pp += size;
     }while(((pp <= (vm->m + vm->b))));

    if(pp > (vm->m + vm->b)){
        segfault(vm);
    }

}

void error(VM *vm, Errorcode e){

    int8 exitcode = -1;

    switch (e)
    {
    case ErrSegv:
        fprintf(stderr, "%s\n", "VM segmentfault fault");
        break;
    case SysHlt:
        fprintf(stderr, "%s\n", "system halted");
        exitcode = 0;
        printf("after ax       = %.04hx\n", $i vm $ax);
        printf("after bx       = %.04hx\n", $i vm $bx);
        printf("after cx       = %.04hx\n", $i vm $cx);
        printf("after dx       = %.04hx\n", $i vm $dx);
        break;
    default:
        break;
    }

    if(vm){
        free(vm);
    }

    exit($i exitcode);
}

VM *virtualMachine(){

    VM *virtual_machine;
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

    /* 
    ex prog:

    mov ax, 0x0005;  0x08 0x05 0x00
    mov bx, 0x0006;  0x09 0x06 0x00
    nop;             0x90
    mov cx, 0x0505;  0x0a 0x05 0x05
    add bx, 0x0006;  0x21 0x06 0x00 
    hlt;             0x01
    */
    // Program *prog;
    int16 size;

    size = map_inst(mov) + map_inst(mov) + map_inst(nop)+ map_inst(mov)+ map_inst(add) + map_inst(hlt);

    Program p[] = {0x08, 0x05, 0x00, 0x09, 0x06, 0x00, 0x10, 0x0a, 0x05, 0x05, 0x21, 0x06, 0x00, 0x18};

    copy($1 vm->m, $1 &p, size);

    vm->b = size;
    vm $ip = (reg)vm->m;
    vm $sp = (reg)-1;

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
    printf("bx       = %.04hx\n", $i vm $bx);
    printf("cx       = %.04hx\n", $i vm $cx);
    printf("dx       = %.04hx\n", $i vm $dx);

    printhex($1 prog, (map_inst(mov) + map_inst(mov) + map_inst(nop)+ map_inst(mov)+ map_inst(add) + map_inst(hlt)), ' ');
    execute(vm);

    return 0;
}