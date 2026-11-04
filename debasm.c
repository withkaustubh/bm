#include "./bm.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage : <./bmi> <program.bm>\n");
        fprintf(stderr, "Error, Proggram file required!");
    }

    Bm bm = {0};
    bm_load_program_from_file(&bm, argv[1]);

    for (size_t i = 0; i < bm.program_size; ++i)
    {
        Inst inst = bm.Program[i];
        switch (inst.type)
        {
        case Inst_Push:
            printf("push %" PRId64 "\n", inst.operand);
            break;
        case Inst_Plus:
            printf("plus\n");
            break;
        case Inst_Minus:
            printf("minus\n");
            break;
        case Inst_Mult:
            printf("mult\n");
            break;
        case Inst_Div:
            printf("div\n");
            break;
        case Inst_Jmp:
            printf("jmp %" PRId64 "\n", inst.operand);
            break;
        case Inst_Halt:
            printf("halt\n");
            break;
        case Inst_Eq:
            printf("eq\n");
            break;
        case Inst_JumpIf:
            printf("jmp_if\n");
            break;
        case Inst_Dup:
            printf("dup %" PRId64 "\n", inst.operand);
            break;
        default:
            fprintf(stderr, "ERROR: Unknown instruction type %d\n", inst.type);
            exit(1);
        }
    }
}