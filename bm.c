#include "./bm.h"

const char *trap_as_cstr(Trap trap)
{
  switch (trap)
  {
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



const char *Inst_as_cstr(Inst inst)
{
  switch (inst.type)
  {
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






Trap bm_execute_inst(Bm *bm)
{
  if (bm->ip < 0 || bm->ip >= bm->program_size)
  {
    return Trap_InvalidInstructionAccess;
  }

  Inst inst = bm->Program[bm->ip];

  switch (inst.type)
  {
  case Inst_Push:
    if (bm->stack_size >= MAX_STACK_CAPACITY)
    {
      return Trap_StackOverflow;
    }
    bm->stack[bm->stack_size++] = inst.operand;
    bm->ip++;
    break;

  case Inst_Plus:
    if (bm->stack_size < 2)
    {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] += bm->stack[bm->stack_size - 1];
    bm->stack_size--;
    bm->ip++;
    break;

  case Inst_Minus:
    if (bm->stack_size < 2)
    {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] -= bm->stack[bm->stack_size - 1];
    bm->stack_size--;
    bm->ip++;
    break;
  case Inst_Mult:
    if (bm->stack_size < 2)
    {
      return Trap_StackUnderflow;
    }
    bm->stack[bm->stack_size - 2] *= bm->stack[bm->stack_size - 1];
    bm->stack_size -= 1;
    bm->ip++;
    break;
  case Inst_Div:
    if (bm->stack_size < 2)
    {
      return Trap_StackUnderflow;
    }
    if (bm->stack[bm->stack_size - 1] == 0)
    {
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
    if (bm->stack_size < 2)
    {
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

void load_program_from_memory(Bm *bm, Inst *program, Word program_size)
{
  if (program_size > BM_PROGRAM_CAPACITY)
  {
    fprintf(stderr, "ERROR: Program size exceeds capacity!\n");
    exit(1);
  }
  memcpy(bm->Program, program, program_size * sizeof(program[0]));
  bm->program_size = program_size;
}

void bm_dump(FILE *stream, Bm *bm)
{
  fprintf(stream, "Stack size: %zu\n", bm->stack_size);
  fprintf(stream, "Stack contents:\n");
  if (bm->stack_size != 0)
    for (Word i = 0; i < bm->stack_size; ++i)
    {
      fprintf(stream, "  [%zu]: %ld\n", i, bm->stack[i]);
    }
  else
    fprintf(stream, "  Empty!\n");
}

void bm_save_program_to_file(Inst *program, size_t program_size,
                             const char *file_path)
{
  FILE *f = fopen(file_path, "wb");

  if (f == NULL)
  {
    fprintf(stderr, "Error Could not open file `%s` : %s", file_path,
            strerror(errno));
    exit(1);
  }

  size_t items_written = fwrite(program, sizeof(program[0]), program_size, f);

  if (items_written != program_size)
  {
    fprintf(stderr, "ERROR: Only wrote %zu out of %ld instructions!\n",
            items_written, program_size);
  }

  if (ferror(f))
  {
    fprintf(stderr, "Error Could not write to  file `%s` : %s", file_path,
            strerror(errno));
    exit(1);
  }
}

void bm_load_program_from_file(Bm *bm, const char *file_path)
{
  FILE *f = fopen(file_path, "rb");
  if (f == NULL)
  {
    fprintf(stderr, "ERROR: Could not open program file `%s`: %s\n",
            file_path, strerror(errno));
    exit(1);
  }

  if (fseek(f, 0, SEEK_END) < 0)
  {
    fprintf(stderr, "ERROR: Could not seek inside file `%s`: %s\n",
            file_path, strerror(errno));
    exit(1);
  }

  long file_size = ftell(f);
  if (file_size < 0)
  {
    fprintf(stderr, "ERROR: Could not read file size of `%s`: %s\n",
            file_path, strerror(errno));
    exit(1);
  }

  if (file_size % sizeof(Inst) != 0)
  {
    fprintf(stderr, "The File you are trying to load is corrupted!");
    exit(1);
  }

  Word program_size = file_size / sizeof(Inst);
  if (program_size > BM_PROGRAM_CAPACITY)
  {
    fprintf(stderr, "ERROR: File program size (%ld) exceeds VM capacity (%d)!\n",
            program_size, BM_PROGRAM_CAPACITY);
    exit(1);
  }

  if (fseek(f, 0, SEEK_SET) < 0)
  {
    fprintf(stderr, "ERROR: Could not rewind file pointer inside `%s`: %s\n",
            file_path, strerror(errno));
    exit(1);
  }

  size_t nread = fread(bm->Program, sizeof(Inst), program_size, f);
  if (nread != (size_t)program_size || ferror(f))
  {
    fprintf(stderr, "Failed to read all complete binary data from file : `%s`\n", file_path);
    exit(1);
  }

  bm->program_size = program_size;
  fclose(f);
}
