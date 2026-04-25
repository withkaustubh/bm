#include <stdio.h>
#include <stdlib.h>

/*
 * Project: experimenting with files
 * Author: Kaustubh
 */

int main(int argc, char *argv[]) {
  FILE *source = fopen("test2.txt", "r+");

  if (source == NULL) {
    fprintf(stderr, "Can't access or open file ");
    exit(1);
  }

  // char buff[100];
  // fgets(buff, 10, source);
  // printf("%s", buff);

  // fprintf(source, "This is a piece of text using fprintf function");
  fputs("This text comes form the fputs function", source);
  fclose(source);
  return 0;
}