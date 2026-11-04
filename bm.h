#ifndef BM_H
#define BM_H

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

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

#define SV_fmt "%.*s"
#define SV_arg(x) (int)x.count, x.data
typedef int64_t Word;

typedef enum
{
  Trap_OK,
  Trap_StackOverflow,
  Trap_StackUnderflow,
  Trap_DivisionByZero,
  Trap_InvalidInstruction,
  Trap_InvalidInstructionAccess

} Trap;

typedef enum INST
{
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

typedef struct
{
  Inst_Types type;
  Word operand;

} Inst;

typedef struct
{
  Word stack[MAX_STACK_CAPACITY];
  Word stack_size;

  Inst Program[BM_PROGRAM_CAPACITY];
  Word program_size;
  Word ip;

  int halt;

} Bm;

const char *trap_as_cstr(Trap trap);
const char *Inst_as_cstr(Inst inst);
Trap bm_execute_inst(Bm *bm);
void load_program_from_memory(Bm *bm, Inst *program, Word program_size);
void bm_dump(FILE *stream, Bm *bm);
void bm_save_program_to_file(Inst *program, size_t program_size,
                             const char *file_path);
void bm_load_program_from_file(Bm *bm, const char *file_path);





#endif //BM_H