[Back to main Page](../README.md)

# Stack Overflow Introduction


# Why Use a Stack?
Why Do We Use A Stack? Modern computers are designed with the need of high-level languages in
mind.  The most important technique for structuring programs introduced by
high-level languages is the procedure or function.  From one point of view, a
procedure call alters the flow of control just as a jump does, but unlike a
jump, when finished performing its task, a function returns control to the 
statement or instruction following the call.  This high-level abstraction
is implemented with the help of the stack.

The stack is also used to dynamically allocate the local variables used in
functions, to pass parameters to the functions, and to return values from the function.

Depending on the implementation the stack will either grow down (towards
lower memory addresses), or up.  In our examples we'll use a stack that grows
down.  This is the way the stack grows on many computers including the Intel, 
Motorola, SPARC and MIPS processors.  The stack pointer (SP) is also
implementation dependent.  It may point to the last address on the stack, or 
to the next free available address after the stack.  For our discussion we'll
assume it points to the last address on the stack.

The first thing a procedure must do when called is save the previous FP
(so it can be restored at procedure exit).  Then it copies SP into FP to 
create the new FP, and advances SP to reserve space for the local variables. 
This code is called the procedure prolog.  Upon procedure exit, the stack 
must be cleaned up again, something called the procedure epilog.  The Intel 
ENTER and LEAVE instructions and the Motorola LINK and UNLINK instructions, 
have been provided to do most of the procedure prolog and epilog work 
efficiently.

# Resources

| Description | URL |
|-------------|-----|
| Fantastic Article on Stack Overflows | [Smashing the Stack for Fun and Profit](https://insecure.org/stf/smashstack.html) |
| Gnu Debugger Resource | [GDB: The GNU Project Debugger](https://www.gnu.org/software/gdb/) |
| Compiler used for C code | [GCC, the GNU Compiler Collection](https://gcc.gnu.org/) |
| Assembler used for this project | [The Yasm Modular Assembler Project](https://yasm.tortall.net/) |
