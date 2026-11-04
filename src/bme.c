#include "./bm.h"

char *shift(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}



int main(int argc, char **argv)
{

    char *program_name = shift(&argc, &argv);
    char *input_file_name = NULL;
    int limit = -1;

    while(argc>0){
        char *flag = shift(&argc, &argv);

        if(strcmp(flag, "-i") == 0){
            if(argc == 0){
                fprintf(stderr, "Error : NO file name provided aftetr -i");
                return 1;
            }
            input_file_name = shift(&argc, &argv);
        }
        else if(strcmp(flag, "-l") == 0){
            if(argc == 0){
                fprintf(stderr, "Error : NO file name provided aftetr -l");
                return 1;
            }
            limit = atoi(shift(&argc, &argv));
        }
        else if(strcmp(flag, "-h") == 0){
            printf("Use tag -i to provide filename and -l to set limit of execution");
            
        }

        else{
            input_file_name = flag;
        }

        
        }

        if(!input_file_name){
            fprintf(stderr, "Error : No program file provided to execute! use -h for more help!");
            return 1;
    }
    
    

    Bm bm = {0};
    bm_load_program_from_file(&bm, input_file_name);
    bm_execute_program(&bm, limit);
    printf("Here is the stack after execution");
    bm_dump(stdout, &bm);

    
    return 0;

    // if (argc < 2)
    // {
    //     fprintf(stderr, "Usage : <./bmi> <program.bm>\n");
    //     fprintf(stderr, "Error, Proggram file required!");
    // }

    // bm_dump(stdout, &bm);
    // for (int i = 0; i < MAX_EXECUTION_LIMIT && !bm.halt; i++)
    // {
    //     printf("\n%s\n", Inst_as_cstr(bm.Program[bm.ip]));
    //     Trap trap = bm_execute_inst(&bm);
    //     // bm_dump(stdout, &bm);
    //     if (trap != Trap_OK)
    //     {
    //         fprintf(stderr, "ERROR: %s\n", trap_as_cstr(trap));
    //         bm_dump(stderr, &bm);
    //         exit(1);
    //     }
    // }
}