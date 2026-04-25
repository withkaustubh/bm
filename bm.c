#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_CAPACITY 1024
#define BM_PROGRAM_CAPACITY 1024
#define ARR_SIZE(x) sizeof(x) / sizeof(x[0])
#define MAX_EXECUTION_LIMIT 30

#define MAKE_INST_PUSH(val) {.type = Inst_Push, .operand = (val)}
#define MAKE_INST_PLUS() {.type = Inst_Plus, .operand = 0}
#define MAKE_INST_Minus() {.type = Inst_Minus, .operand = 0}
#define MAKE_INST_Mult() {.type = Inst_Mult, .operand = 0}
#define MAKE_INST_Div() {.type = Inst_Div, .operand = 0}
#define MAKE_INST_JMP(addr) {.type = Inst_Jmp, .operand = (addr)}
#define MAKE_INST_HALT() {.type = Inst_Halt, .operand = 0}
#define MAKE_INST_Eq() {.type = Inst_Eq, .operand = 0}
#define MAKE_INST_JumpIf(addr) {.type = Inst_JumpIf, .operand = (addr)}
#define MAKE_INST_Dup(addr) {.type = Inst_Dup, .operand = (addr)}
typedef int64_t Word;

typedef enum {
  Trap_OK,
  Trap_StackOverflow,
  Trap_StackUnderflow,
  Trap_DivisionByZero,
  Trap_InvalidInstruction,
  Trap_InvalidInstructionAccess

} Trap;

const char *trap_as_cstr(Trap trap) {
  switch (trap) {
  case (Trap_OK):
    return "Trap_Ok";
  case Trap_StackOverflow:
    return "Trap_StackOverflow";
  case Trap_StackUnderflow:
    return "Trap_StackUnderflow";
  case Trap_DivisionByZero:
    return "Trap_DivisionByZero";
  case Trap_InvalidInstruction:
    return "Trap_InvalidInstruction";
  case Trap_InvalidInstructionAccess:
    return "Trap_InvalidInstructionAccess";
  default:
    assert(0 && "Unreachable");
    break;
  }
}

typedef enum INST {
  Inst_Push,
  Inst_Plus,
  Inst_Minus,
  Inst_Mult,
  Inst_Div,
  Inst_Jmp,
  Inst_Halt,
  Inst_Eq,
  Inst_JumpIf,
  Inst_Dup

} Inst_Types;

typedef struct {
  Inst_Types type;
  Word operand;

} Inst;

const char *Inst_as_cstr(Inst inst) {
  switch (inst.type) {
  case Inst_Push:
    return "Inst_Push";
    break;
  case Inst_Plus:
    return "Inst_Plus";
    break;
  case Inst_Minus:
    return "Inst_Minus";
    break;
  case Inst_Mult:
    return "Inst_Mult";
    break;
  case Inst_Div:
    return "Inst_Div";
    break;
  case Inst_Jmp:
    return "Inst_Jmp";
    break;
  case Inst_Halt:
    return "Inst_Halt";
    break;
  case Inst_Eq:
    return "Inst_Eq";
    break;
  case Inst_JumpIf:
    return "Inst_JumpIf";
    break;
  case Inst_Dup:
    return "Inst_Dup";
    break;
  default:
    return "Invalid instruction";
    break;
  }
}

typedef struct {
  Word stack[MAX_STACK_CAPACITY];
  Word stack_size;

  Inst Program[BM_PROGRAM_CAPACITY];
  Word program_size;
  Word ip;

  int halt;

} Bm;

Trap bm_execute_inst(Bm *bm) {
  if (bm->ip < 0 || bm->ip >= bm->program_size) {
    return Trap_InvalidInstructionAccess;
  }

  Inst inst = bm->Program[bm->ip];

  switch (inst.type) {
  case Inst_Push:
    if (bm->stack_size >= MAX_STACK_CAPACITY) {
      return Trap_StackOverflow;
    }
    bm->stack[bm->stack_size++] = inst.operand;
    bm->ip++;
    break;

  case Inst_Plus:
    if (bm->stack_size < 2) {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] += bm->stack[bm->stack_size - 1];
    bm->stack_size -= 1;
    bm->ip++;
    break;

  case Inst_Minus:
    if (bm->stack_size < 2) {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] -= bm->stack[bm->stack_size - 1];
    bm->stack_size -= 1;
    bm->ip++;
    break;
  case Inst_Mult:
    if (bm->stack_size < 2) {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] *= bm->stack[bm->stack_size - 1];
    bm->stack_size -= 1;
    bm->ip++;
    break;
  case Inst_Div:
    if (bm->stack_size < 2) {
      return Trap_StackUnderflow;
    }
    if (bm->stack[bm->stack_size - 1] == 0) {
      return Trap_DivisionByZero;
    }
    bm->stack[bm->stack_size - 2] /= bm->stack[bm->stack_size - 1];
    bm->stack_size -= 1;
    bm->ip++;
    break;

  case Inst_Halt:
    bm->halt = 1;
    break;

  case Inst_Jmp:
    bm->ip = inst.operand;
    break;

  case Inst_Eq:
    if (bm->stack_size < 2) {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] =
        bm->stack[bm->stack_size - 2] == bm->stack[bm->stack_size - 1];
    bm->stack_size--;
    bm->ip++;
    break;
  case Inst_JumpIf:
    if (bm->stack_size < 1)
      return Trap_StackUnderflow;
    if (bm->stack[--bm->stack_size]) // also pops the boolean value
      bm->ip = inst.operand;
    else
      bm->ip++;
    break;
  case Inst_Dup:

    if (bm->stack_size >= MAX_STACK_CAPACITY)
      return Trap_StackOverflow;

    if (bm->stack_size - inst.operand <= 0)
      return Trap_StackUnderflow;
    int idx_to_dup = bm->stack_size - inst.operand - 1;
    bm->stack[bm->stack_size++] = bm->stack[idx_to_dup];
    bm->ip++;

    break;

  default:
    return Trap_InvalidInstruction;
    break;
  }
  return Trap_OK;
}

void load_program_from_memory(Bm *bm, Inst *program, Word program_size) {
  if (program_size > BM_PROGRAM_CAPACITY) {
    fprintf(stderr, "ERROR: Program size exceeds capacity!\n");
    exit(1);
  }
  memcpy(bm->Program, program, program_size * sizeof(program[0]));
  bm->program_size = program_size;
}

void bm_dump(FILE *stream, Bm *bm) {
  fprintf(stream, "Stack size: %zu\n", bm->stack_size);
  fprintf(stream, "Stack contents:\n");
  if (bm->stack_size != 0)
    for (Word i = 0; i < bm->stack_size; ++i) {
      fprintf(stream, "  [%zu]: %ld\n", i, bm->stack[i]);
    }
  else
    fprintf(stream, "  Empty!\n");
}

void bm_save_program_to_file(Inst *program, Word program_size,
                             const char *file_path) {
  FILE *f = fopen(file_path, "wb");

  if (f == NULL) {
    fprintf(stderr, "Error Could not open file `%s` : %s", file_path,
            strerror(errno));
    exit(1);
  }

  fwrite(program, sizeof(program[0]), program_size, f);

  if (ferror(f)) {
    fprintf(stderr, "Error Could not write to  file `%s` : %s", file_path,
            strerror(errno));
    exit(1);
  }
}

Inst Program[] = {
    // MAKE_INST_PUSH(1),
    // MAKE_INST_PUSH(2),
    // MAKE_INST_PUSH(3),
    // MAKE_INST_PLUS(),
    // MAKE_INST_PUSH(4),
    // MAKE_INST_PUSH(5),
    // MAKE_INST_Minus(),
    // MAKE_INST_Div(),
    // MAKE_INST_Mult(),
    // MAKE_INST_HALT()

    // MAKE_INST_PUSH(10), // 0
    // MAKE_INST_PUSH(20), // 1
    // MAKE_INST_PLUS(),   // 2
    // MAKE_INST_JMP(1)

    // writing fibonacci program
    MAKE_INST_PUSH(0), // 0: a = 0
    MAKE_INST_PUSH(1), // 1: b = 1

    MAKE_INST_Dup(1), // 2
    MAKE_INST_Dup(1), // 3
    MAKE_INST_PLUS(), // 4
    MAKE_INST_JMP(2)  // 5

    // MAKE_INST_PUSH(10),
    // MAKE_INST_PUSH(10),
    // MAKE_INST_PUSH(10),
    // MAKE_INST_PUSH(10),
    // MAKE_INST_PUSH(10),
    // MAKE_INST_PUSH(10)

};

Bm bm = {0};
int main() {
  load_program_from_memory(&bm, Program, ARR_SIZE(Program));
  bm_save_program_to_file(bm.Program, bm.program_size, "fibonacci.bm");
  bm_dump(stdout, &bm);
  for (int i = 0; i < MAX_EXECUTION_LIMIT && !bm.halt; i++) {
    printf("\n%s\n", Inst_as_cstr(Program[bm.ip]));
    Trap trap = bm_execute_inst(&bm);
    bm_dump(stdout, &bm);
    if (trap != Trap_OK) {
      fprintf(stderr, "ERROR: %s\n", trap_as_cstr(trap));
      bm_dump(stderr, &bm);
      exit(1);
    }
  }

  bm_dump(stdout, &bm);
  return 0;
}
