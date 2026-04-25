#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project: Automate reciept
 * Author: Kaustubh
 */

char * replace(char *source, const char *tag, const char *replacment);

int main(int argc, char *argv[])
{

    FILE *source = fopen("letter.txt", "r+");
    if (source==NULL){
     fprintf(stderr, "Can't access or open file ");
     exit(1);
    }

    char name[32], outlet[32], message[64];
    printf("Enter user name : ");
    scanf("%31s", name);
    printf("Enter outlet name : ");
    scanf("%31s", outlet);
    printf("Enter greet message : ");
    scanf("%63s", message);

    char *result = replace((char*)source, "name", name);
    result = replace((char*)result,  "outlet", outlet);
    result = replace((char*)result,  "message", message);
    fclose(source);
    return 0;
}

char* replace(char *source, const char *tag, const char *replacment)
{
    char * tag_pos = strstr(source, tag);
    int tag_len = strlen(tag);
    int prefix_len = tag_pos - source;
    int rep_len = strlen(replacment);

    int suffix_start = prefix_len + tag_len;

    int total_len = (strlen(source) - tag_len) + rep_len + 1;

    char* result = (char *)malloc(total_len);

    strncat(result, source, prefix_len);
    result[prefix_len] = '\0';

    strcat(result, replacment);

    strcat(result, source + suffix_start);

    return result;

}
