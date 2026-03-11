## Steps to create virtual machine from scratch in create

1. Create a dataType Word as the basic for start
2. create a stack with a max capacity to hold Word that will be our virtual machine
3. create a single function to execute commands on the bm
4. initial commands or inst_types for basic start
5. basic error handling using enum Trap


NOTE (A WAY TO CREATE PROGRAM)

```c Inst program[1024];
int program_size = 0;

void emit(Inst i) {
    program[program_size++] = i;
}

// Inside main or a setup function:
emit(inst_push(1));
emit(inst_push(2));
emit(inst_add());
```

**Step**: add the functionalitiies for minus, mult and divisin in inst

inst jump if
inst eq
inst_print_debug
inst_dup

Fibonacci
bm save program to file

bm load program from file
