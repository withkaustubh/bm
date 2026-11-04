#include "./bm.h"
#include "./sv.h"

Inst bm_translate_line(String_view line)
{
  line = sv_trim_left(line);

  String_view inst_name = sv_chop_by_delim(&line, ' ');

  if (sv_equal(inst_name, sv_from_cstr("push")))
  {
    line = sv_trim(line);
    Word operand = sv_to_int(line);
    return (Inst)MAKE_INST_PUSH(operand);
  }
  else if (sv_equal(inst_name, sv_from_cstr("plus")))
  {
    return (Inst)MAKE_INST_PLUS();
  }
  else if (sv_equal(inst_name, sv_from_cstr("minus")))
  {
    return (Inst)MAKE_INST_Minus();
  }
  else if (sv_equal(inst_name, sv_from_cstr("mult")))
  {
    return (Inst)MAKE_INST_Mult();
  }
  else if (sv_equal(inst_name, sv_from_cstr("div")))
  {
    return (Inst)MAKE_INST_Div();
  }
  else if (sv_equal(inst_name, sv_from_cstr("jmp")))
  {
    line = sv_trim(line);
    Word operand = sv_to_int(line);
    return (Inst)MAKE_INST_JMP(operand);
  }
  else if (sv_equal(inst_name, sv_from_cstr("halt")))
  {
    return (Inst)MAKE_INST_HALT();
  }
  else if (sv_equal(inst_name, sv_from_cstr("eq")))
  {
    return (Inst)MAKE_INST_Eq();
  }
  else if (sv_equal(inst_name, sv_from_cstr("jmp_if")))
  {
    line = sv_trim(line);
    Word operand = sv_to_int(line);
    return (Inst)MAKE_INST_JumpIf(operand);
  }
  else if (sv_equal(inst_name, sv_from_cstr("dup")))
  {
    line = sv_trim(line);
    Word operand = sv_to_int(line);
    return (Inst)MAKE_INST_Dup(operand);
  }
  else
  {
    fprintf(stderr, "Unkown Instruction " SV_fmt "\n", SV_arg(inst_name));
    exit(1);
  }
}

int bm_translate_source(Bm *bm, String_view *source, long Program_capacity)
{
  bm->program_size = 0;
  while (source->count != 0)
  {
    String_view line = sv_chop_by_delim(source, '\n');
    line = sv_trim(line);

    if (line.count == 0 || line.data[0] == '#')
    {
      // igonre this line and move on
      continue;
    }

    if (bm->program_size >= Program_capacity)
    {
      fprintf(stderr, "Error : Program Maximum Capacity reached!");
      exit(1);
    }

    bm->Program[bm->program_size++] = bm_translate_line(line);
    printf("Added line " SV_fmt "\n", SV_arg(line));
  }
  return bm->program_size;
}

String_view slurp(const char *file_path)
{
  FILE *f = fopen(file_path, "r");
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

  long m = ftell(f);
  if (m < 0)
  {
    fprintf(stderr, "ERROR: Could not read file size of `%s`: %s\n",
            file_path, strerror(errno));
    exit(1);
  }

  rewind(f);
  char *buffer = malloc(m);

  if(!buffer){
    fprintf(stderr, "Error : Buffer allocation failed for file : %s", file_path);
    exit(1);
  }

  fread(buffer,1, m, f);
  if(ferror(f)){
    fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
            file_path, strerror(errno));
    exit(1);
  }
  fclose(f);
  return (String_view) {.count = m, .data = buffer};
  
}

int main(int argc, char** argv){
  if(argc < 3){
    fprintf(stderr, "Usage : <./bm> <input.ebasm> <output.bm> \n");
    fprintf(stderr , "Error : input and output are expected!\n");
  }

  const char* input_file = argv[1];
  const char* output_file = argv[2];
  Bm bm = {0};
  String_view source = slurp(input_file);
  bm_translate_source(&bm, &source, BM_PROGRAM_CAPACITY);
  bm_save_program_to_file(bm.Program, bm.program_size, output_file);

  return 0;
}