# SIA-Architecture
 Custom Built Computer Architecture for Machine Code

This program uses C to make a custom machine code assembler and compiler. A test case is provided to be used with the assembler and virtual machine. The assembler takes user inputs from the test cases and turns into hex to be read by the virtual machine. The VM will proceed to fetch, dispatch, execute, and store the data.

## Installation

Install is as simple as downloading and running with visual studios. Although a command terminal with GCC will be needed to read the hex data that is printed out.

GCC installation guide for windows: https://www.youtube.com/watch?v=sXW2VLrQ3Bs
Macs can use "command line: od -x" for hex dump.

## Technologies

Microsft Visual Studios 2019: C

## Instruction Set Functionality

(opcode is written in parentheses, the format is written after | )
add(1): Adds two registers together and stores to the first register | add r# r# r#
addimmediate(9): Adds an integer to register | addimmediate r# #
and(2): Bitwise Ands two registers and stores into data into the first register | and r# r# r#
branchifless(11): Branches if less by designated offset | branchifless r# r# OFFSET#
branchifequal(10): Branches if equal by designated offset | branchifequal r# r# OFFSET#
divide(3): Divides two registers and stores data into first given register | divide r# r# r#
halt(0): Ends the program | halt
interrupt(8): Used to print out values in registers | interrupt 0
jump(12): Used to jump by number of bits | jump # 
leftshift(7): Left shifts register value bitwise by the number written after | leftshift r# #
multiply(4): Multiplies two registers together and saves the value to the first register | multiply r# r# r#
or(6): Does a bitwise Or of two registers storing data onto first register | Or r# r# r#
rightshift(7): Right shifts register value bitwise by the number written after | rightshift r# #
subtract(5): Subtracts two registers together and stores to the first register | sub r# r# r#
