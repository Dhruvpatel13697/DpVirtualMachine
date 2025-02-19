# DP's Virtual Machine

## Overview
DP's Virtual Machine (VM) is a basic 16-bit virtual machine designed to execute a single program at a time. It features a stack size of 65,535 and supports several fundamental instructions such as MOV, ADD, PUSH, POP, NOP, and HLT.

## Features
- **16-bit architecture**
- **Single-program execution**
- **Basic instruction set** including MOV, ADD, PUSH, POP, etc.
- **Registers**: AX, BX, CX, DX, SP, IP
- **Memory divided into .text (program) and stack (runtime storage)**

## Instructions

### Opcode Table
| Instruction | Opcode  | Binary Representation | Size (Bytes) |
|------------|--------|----------------------|--------------|
| MOV        | 0x08   | 0000 1000            | 3            |
| NOP        | 0x10   | 0001 0000            | 1            |
| HLT        | 0x18   | 0001 1000            | 1            |
| ADD        | 0x20   | 0010 0000            | 3            |
| PUSH       | 0x28   | 0010 1000            | 3            |
| POP        | 0x30   | 0011 0000            | 1            |
| jmp        | 0x38   | 0011 1000            | 3            |
### Register Table
| Register | Number | Binary Representation |
|----------|--------|----------------------|
| AX       | 0x00   | 0000 0000            |
| BX       | 0x01   | 0000 0001            |
| CX       | 0x02   | 0000 0010            |
| DX       | 0x03   | 0000 0011            |
| SP       | 0x04   | 0000 0100            |
| IP       | 0x05   | 0000 0101            |
| flg      | 0x06   | 0000 0110            |

## Instruction Structure (Immediate Mode Only)

### Example 1: MOV Instruction
```assembly
mov ax, 0x0005
```
| Opcode | Register | Immediate (Little Endian) |
|--------|----------|--------------------------|
| 0x08   | 0x00     | 0x05 0x00                |
**Final instruction**: `0x08 0x05 0x00` (3 bytes)

### Example 2: ADD Instruction
```assembly
add bx, 0x0506
```
| Opcode | Register | Immediate (Little Endian) |
|--------|----------|--------------------------|
| 0x20   | 0x01     | 0x06 0x05                |
**Final instruction**: `0x21 0x06 0x05` (3 bytes)

### Example 3: PUSH Instruction
```assembly
push 0x0506
```
| Opcode | Immediate (Little Endian) |
|--------|--------------------------|
| 0x28   | 0x06 0x05                |
**Final instruction**: `0x28 0x06 0x05` (3 bytes)

This instruction pushes `0x0506` onto the stack and decreases the stack pointer (SP) by the size of `0x0506`.

## Memory Layout
The memory is divided into two sections:
1. **.text** - Holds the program instructions.
2. **Stack** - Stores runtime values (e.g., pushed values).

```
_______________________________________________________________________________
|                       |                                                     |
|                       |                                                     |
-------------------------------------------------------------------------------
   .text                ^                 stack                               ^
                    break line                                                sp
```

## Usage
- Load a program into the **.text** section.
- Execute instructions sequentially.
- The stack operates separately for runtime data storage.


