#include <math.h>
#include <stdio.h>
#include <string.h>

/*
 * Project: count number of lines, words and characters is a file
 * Author: Kaustubh
 */

int nchars(char *text);
int nwords(char *text);
int nlines(char *text);

int main(int argc, char *argv[]) {

  printf("This program prints number of lines in this file");

  FILE *ptr = fopen("temp/log6.txt", "r");
}

int nchars(char *buffer) { return strlen(buffer); }

int nwords(char *text) {
  int count = 0;
  int in_word = 0;
  for (int i = 0; i < sizeof(text); i++) {

    //   printf("%zu", strlen(text));
    for (int i = 0; i < strlen(text); i++) {
      char *whtiteset = " \t\n";

      if (!strchr(whtiteset, text[i])) {
        if (!in_word) {
          count++;
          in_word = 1;
        }
      } else
        in_word = 0;
    }
  }
  return count;
}

int nlines(char *text) {
  int count = 0;
  for (int i = 0; i < strlen(text); i++) {
    if (i == '\n')
      count++;
  }
  return count;
}