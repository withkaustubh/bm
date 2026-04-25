#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Project: taking user input and save it in a log file
 * Author: Kaustubh
 */

int main(int argc, char *argv[]) {
  printf("\nWelcome to logger build by kaustubh Agarwal ------- \n");
  int ctr = 0;
  char *buffer = NULL;
  size_t buffer_size = 0;
  while (1) {

    char filename[64];
    snprintf(filename, sizeof(filename), "%s%d.txt", "temp/log", ++ctr);
    char mode[10] = "w";

    if (access(filename, F_OK) == 0) {
      while (1) {
        printf("The file %s already exists, type a(append) or w(overwrite): ",
               filename);
        scanf(" %1s", mode);

        // Clean up the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;

        if (strcmp(mode, "a") == 0 || strcmp(mode, "w") == 0) {
          break;
        }
        printf("Invalid choice. ");
      }
    }

    // char buffer[256];

    printf("Enter content for %s: (type {exit} to exit) ", filename);

    // scanf("%s", buffer);
    // fgets(buffer, sizeof(buffer), stdin);
    getline(&buffer, &buffer_size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if (strcmp(buffer, "{exit}") == 0) {
      printf("Ending the program...");
      break;
    }

    FILE *fptr = fopen(filename, mode);
    if (fptr == NULL) {
      perror("Can't access file");
      continue;
    }

    fputs(buffer, fptr);
    fclose(fptr);
  }
  free(buffer);
  return 0;
}