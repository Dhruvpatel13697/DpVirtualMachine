/* PROJECTNAME.c */
#include "dp_virtual_machin.h"

VM *vrtualMachine(Program *prog, int16 prog_size){

    VM *p;
    Program *prog_pointer;
    int16 size;

    assert((prog) && (prog_size));

    size = $2 sizeof(struct s_vm);
    p = (VM*)(malloc($i size));

    if(!p){
        errno = ErrMem;
        return (VM *)0;  //return empty VM
    };

    zero($1 p, size);

    prog_pointer = (Program*)(malloc($i prog_size));
    if(!prog_pointer){
        free(p);
        errno = ErrMem;
        return (VM *)0;
    };
}

int main(int argc, char *argv[]) {
    return 0;
}