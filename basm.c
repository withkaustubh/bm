#include "./bm.h"
#include "./sv.h"

typedef struct {
    String_view name;
    size_t address;
} Label;

typedef struct {
    Label items[1024];
    size_t count;
} Label_Table;

typedef struct {
    size_t inst_addr; 
    String_view label_name;   
} Deferred_Operand;

#define MAX_DEFERRED 1024

typedef struct {
    Deferred_Operand items[MAX_DEFERRED];
    size_t count;
} Deferred_Table;

Label_Table lt;
Deferred_Table dt;

int lt_find_label(Label_Table *lt, String_view name, size_t *out_address){
  for(size_t i = 0; i < lt->count; ++i){
    if(sv_equal(lt->items[i].name, name)){
      if(out_address){
        *out_address = lt->items[i].address;
      }
      return 1;
    }
  }
  return 0;
}

Inst bm_translate_line(String_view line, Bm *bm)
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
    line = sv_trim_left(line);
    sv_chop_by_delim(&line, ' ');
    line = sv_trim(line); // now line stores label name;
    dt.items[dt.count++] = (Deferred_Operand){.inst_addr = bm->program_size, .label_name = line};

    return (Inst){.type = Inst_Jmp, .operand = 0};
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

int bm_translate_source(Bm *bm, String_view *source, size_t Program_capacity)
{
  bm->program_size = 0;
  while (source->count != 0)
  {
    String_view line = sv_chop_by_delim(source, '\n');
    line = sv_trim(line);
    String_view code = sv_chop_by_delim(&line, '#');
    if (code.count == 0)
    {
      // igonre this line and move on
      continue;
    }

    if (bm->program_size >= Program_capacity)
    {
      fprintf(stderr, "Error : Program Maximum Capacity reached!");
      exit(1);
    }
    if(code.data[code.count -1] == ':'){
      String_view lab = sv_chop_by_delim(&code, ':');
      lab = sv_trim(lab);
      lt.items[lt.count++] = (Label){.name = lab.data, .address = bm->program_size};
      continue;
    }

    bm->Program[bm->program_size++] = bm_translate_line(code, bm);
    printf("Added line " SV_fmt "\n", SV_arg(code));
  }

  while(dt.count > 0){
    Deferred_Operand jmp_stat = dt.items[dt.count--];
    int ind_to_jmp = lt.lt_find_label(jmp_stat.label_name);
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
    fprintf(stderr, "Usage : <./bm> <input.basm> <output.bm> \n");
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