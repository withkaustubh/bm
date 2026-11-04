#include "./bm.h"
int main(int argc, char **argv)
{
    if(argc < 2){
        fprintf(stderr, "Usage : <./bmi> <proogram.bm>\n");
        fprintf(stderr, "Error, Proggram file required!");
    }

    Bm bm = {0};
    bm_load_program_from_file(&bm, argv[1]);
    // bm_dump(stdout, &bm);
    for (int i = 0; i < MAX_EXECUTION_LIMIT && !bm.halt; i++)
    {
        printf("\n%s\n", Inst_as_cstr(bm.Program[bm.ip]));
        Trap trap = bm_execute_inst(&bm);
        // bm_dump(stdout, &bm);
        if (trap != Trap_OK)
        {
            fprintf(stderr, "ERROR: %s\n", trap_as_cstr(trap));
            bm_dump(stderr, &bm);
            exit(1);
        }
    }
    printf("This is final stack after the program is terminated : \n");
    bm_dump(stdout, &bm);
    return 0;
}