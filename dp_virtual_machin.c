/* PROJECTNAME.c */
#include "dp_virtual_machin.h"

reg *get_reg_addr(VM *vm, int8 reg_num){
    return &vm $ax + $2 reg_num;
}

void _mov(VM * vm, int8 dest_reg, int16 arg){

    reg *dest_r = get_reg_addr(vm, dest_reg);

    *dest_r = arg;

    return;

}


void _add(VM * vm, int8 dest_reg, Args a1, Args a2){
    reg r = ((a2 << 8) + a1);
    reg *dest_r = get_reg_addr(vm, dest_reg);

    *dest_r += r;

    return;
}

void _push(VM *vm, int16 arg){
    *(vm->m + vm $sp) = arg;
    vm $sp -= sizeof(int16);
}

void _pop(VM *vm, int8 dest_reg){
    vm $sp += sizeof(int16);
    int16 r = get_value_from_offset(vm, vm $sp);
    
    reg *dest_r = &vm $ax + $2 dest_reg;

    *dest_r = r;

    return;
}
void _jmp(VM *vm, Args a1, Args a2){
    vm $ip = ((a2 << 8) + a1);
}

void _cmp(VM *vm, int8 dest_reg, Args a1, Args a2){
    vm $flg = 0x0000;
    int16 r = ((a2 << 8) + a1);
    
    reg *dest_r = &vm $ax + $2 dest_reg;

    if(dest_reg < 0x04){
        if(*dest_r == r){
            vm $flg = vm $flg | 0x0001; // 0000 0000 0000 0001
        }
        else if(*dest_r < r){
            vm $flg = vm $flg | 0x0002; // 0000 0000 0000 0010 
        }
        else if(*dest_r > r){
            vm $flg = vm $flg | 0x0004; // 0000 0000 0000 0100
        }   
    }
    
    else{
        segfault(vm);
    }

    return;
}

void _call(VM *vm, Args a1, Args a2){

    int16 base_addr = vm $bp;
    _push(vm, vm $ip + map_inst(call));        // push ip
    vm $bp = vm $sp;                           // mov sp into bp (create new fram)
    _push(vm, base_addr);                      // push base addr
    _jmp(vm, a1, a2);                          // jmp on target addr

}

void _ret(VM *vm){
    int16 base_addr = get_value_from_offset(vm, vm $bp);
    vm $sp = vm $bp;               // mov bp into sp
    _mov(vm, bp, base_addr);       // mov base_addr in bp
    _pop(vm, ip);                  // pop value of ip

}

int16 get_value_from_offset(VM *vm, int16 offset){
    return *(vm->m + offset);
}

void execute(VM *vm){
     Instruction *ip;
     Program *pp;
     int16 size = 0, arg;
     Opcode opcode;
     int8 dest_reg;

     assert(vm && *vm->m);
     pp = vm->m;

     do{
        opcode = *(pp + vm $ip) & (0xf8);
        printf("opcode = %.02hx, inst = %.02hx",$i opcode, $i *(pp + vm $ip));
        switch (opcode)
        {
            case (Opcode)mov:
                printf("\ninst = mov\n");
                size = map_inst(mov);
                ip = copy_instruction((pp + vm $ip), size);
                dest_reg = *(pp + vm $ip) & 0x07;
                arg = ((ip->a[1] << 8) + ip->a[0]);
                _mov(vm, dest_reg, arg);
                break;
            
            case (Opcode)add:
                printf("\ninst = add\n");
                size = map_inst(add);
                ip = copy_instruction((pp + vm $ip), size);
                dest_reg = *(pp + vm $ip) & 0x07;
                _add(vm, dest_reg, ip->a[0], ip->a[1]);
                break;

            case (Opcode)push:
                printf("\ninst = push\n");
                size = map_inst(push);
                ip = copy_instruction((pp + vm $ip), size);
                arg = ((ip->a[1] << 8) + ip->a[0]);
                _push(vm, arg);
                break;

            case (Opcode)pop:
                printf("\ninst = pop\n");
                size = map_inst(pop);
                ip = copy_instruction((pp + vm $ip), size);
                dest_reg = *(pp + vm $ip) & 0x07;
                _pop(vm, dest_reg);
                break;

            case (Opcode)jmp:
                printf("\ninst = jmp\n");
                size = map_inst(jmp);
                ip = copy_instruction((pp + vm $ip), size);
                _jmp(vm, ip->a[0], ip->a[1]);
                break;

            case (Opcode)cmp:
                printf("\ninst = cmp\n");
                size = map_inst(cmp);
                ip = copy_instruction((pp + vm $ip), size);
                dest_reg = *(pp + vm $ip) & 0x07;
                _cmp(vm, dest_reg, ip->a[0], ip->a[1]);
                break;

            case (Opcode)call:
                printf("\ninst = call\n");
                size = map_inst(cmp);
                ip = copy_instruction((pp + vm $ip), size);
                _call(vm, ip->a[0], ip->a[1]);
                break;

            case (Opcode)ret:
                printf("\ninst = ret\n");
                size = map_inst(ret);
                ip = copy_instruction((pp + vm $ip), size);
                _ret(vm);
                break;

            case (Opcode)nop:
                printf("\ninst = nop\n");
                size = map_inst(nop);
                break;

            case (Opcode)hlt:
                printf("\nhlt\n");
                error(vm, SysHlt);
                break;
            
            default:
                segfault(vm);
                break;
        }

        if(opcode != (Opcode)jmp && opcode != (Opcode)call && opcode != (Opcode)ret){
            vm $ip += size;
        }

        printf("--------------------------------\n");
        printf("ax       = %.04hx\n", $i vm $ax);
        printf("bx       = %.04hx\n", $i vm $bx);
        printf("cx       = %.04hx\n", $i vm $cx);
        printf("dx       = %.04hx\n", $i vm $dx);
        printf("sp       = %.04hx\n", $i vm $sp);
        printf("bp       = %.04hx\n", $i vm $bp);
        printf("ip       = %.04hx\n", $i vm $ip);
        printf("flg      = %.04hx\n", $i vm $flg);

     }while((((pp + vm $ip) <= (vm->m + vm->b))));

    if(pp > (vm->m + vm->b)){
        segfault(vm);
    }

}

Instruction *copy_instruction(Program *prog, int16 size){
    Instruction *ip;
    printhex($1 prog, size, ' ');
    ip = (Instruction *)malloc($i size);
    zero($1 ip, size);
    copy($1 ip, $1 prog, size);

    return ip;

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
    int32 size;

    size = sizeof(VM);
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

0x00    mov ax, 0x0005;  0x08 0x05 0x00
0x03    push 0x07;       0x28 0x07 0x00
0x06    call 0x0022;     0x48 0x22 0x00  
0x09    pop dx;          0x33
0x0a    mov bx, 0x0006;  0x09 0x06 0x00
0x0d    nop;             0x10
0x0e    jmp 0x0f         0x38 0x0f 0x00
0x11    hlt;             0x18
0x12    mov cx, 0x0505;  0x0a 0x05 0x05
0x15    cmp bx, 0x0006;  0x41 0x06 0x00
0x18    add bx, 0x0007;  0x21 0x07 0x00 
0x1b    add cx, 0x0007;  0x22 0x07 0x00
0x1e    hlt;             0x18
0x1f    nop;             0x10
0x20    nop;             0x10
0x21    add ax, 0x0006;  0x20 0x06 0x00
0x24    push 0x08;       0x28 0x07 0x00
0x27    pop ax;          0x30
0x28    ret;             0x50
    */
    
    Program p[] = {
        0x08, 0x05, 0x00, 
        0x28, 0x07, 0x00,
        0x48, 0x21, 0x00, 
        0x33, 
        0x09, 0x06, 0x00, 
        0x10, 
        0x38, 0x0f, 0x00,
        0x18,
        0x0a, 0x05, 0x05,
        0x41, 0x06, 0x00,
        0x21, 0x07, 0x00, 
        0x22, 0x07, 0x00, 
        0x18,
        0x10,
        0x10,
        0x20, 0x06, 0x00,
        0x28, 0x07, 0x00,
        0x30,
        0x50

    };

    printf("size p = %ld\n", sizeof(p));
    copy($1 &vm->m, $1 &p, sizeof(p));

    printhex($1 &vm->m, sizeof(p), ' ');

    vm->b = sizeof(p);
    vm $ip = (reg)0;
    vm $bp = (reg)-1;
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
    execute(vm);

    return 0;
}