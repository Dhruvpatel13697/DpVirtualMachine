# DpVirtualMachine
This is DP's virtual machine.

It is a basic 16-bit virtual machine with a stack size of 65,535, capable of executing instructions such as MOV, ADD, PUSH, POP, NOP, HLT, etc.

This is a single-program VM, meaning it can execute only one program at a time.

Additionally, this VM includes registers such as AX, BX, CX, DX, SP, IP etc.

opcode numbering                    total size (immediate only)

MOV  =  0x08  =  0000 1000              (3 Byte)
NOP  =  0x10  =  0001 0000              (1 Byte)
HLT  =  0x18  =  0001 1000              (1 Byte)
ADD  =  0x20  =  0010 0000              (3 Byte)
PUSH =  0x28  =  0010 1000              (3 Byte)
POP  =  0x30  =  0010 0000              (1 Byte)

register numbering

AX = 0x00 = 0000 0000
BX = 0x01 = 0000 0001
CX = 0x02 = 0000 0010
DX = 0x03 = 0000 0011
SP = 0x04 = 0000 0100
IP = 0x05 = 0000 0101

instruction structure(implement immidiate only)

ex 1:

    mov ax, 0x0005
    |-----||------|
    1 B    3 B
    {
        (mov = 0x08 ,ax = 0x00) => 1 Byte   => 0x08 + 0x00 => 0x08 => 0000 1000
        (args = 0x05) => 2 Byte littel endian => 0x05 0x00 => 0000 0101 0000 0000

        final inst => 0x08 0x05 0x00 (3 Byte)
    }

ex 2:

    add bx, 0x0506
    |-----||------|
      1 B    3 B
    {
        (add = 0x20 ,bx = 0x01) => 1 Byte   => 0x20 + 0x01 => 0x21 => 0010 0001
        (args = 0x0506) => 2 Byte littel endian => 0x06 0x05 => 0000 0101 0000 0110

        final inst => 0x21 0x06 0x05 (3 Byte)
    }

ex 3:

    push 0x0506   
    |--| |-----|
     1B    3B
    {
        (push = 0x28) => 1 Byte   => 0x28 => 0010 1000
        (args = 0x0506) => 2 Byte littel endian => 0x06 0x05 => 0000 0101 0000 0110

        final inst => 0x28 0x06 0x05 (3 Byte)
    }

    This instruction pushes 0x0506 onto the stack and decreases the stack pointer (SP) by the size of 0x0506.



How Memory divided :

memory divided into 2 parts first .text where we loads our program and secone part is stack where we store value at run time like push 0x0506.

ex: 
-------------------------------------------------------------------------------
|                       |                                                     |
|                       |                                                     |
-------------------------------------------------------------------------------
   .text                ^                 stack                               ^
                    break line                                                sp


