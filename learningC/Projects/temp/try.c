#include <stdio.h>
#include <string.h>

/*
 * Project: testing project

 * Author: Kaustubh
 */

int main(int argc, char *argv[]) {
  char text[128] = "hello world\nThis is 12 3 3  \n";
  int count = 0;
  int in_word = 0;
  //   printf("%zu", strlen(text));
  for (int i = 0; i < strlen(text); i++) {
    char *whtiteset = " \t\n";

    if (!strchr(whtiteset, text[i])) {
      if (!in_word) {
        count++;
        in_word = 1;
      }
    } else {
      in_word = 0;
    }
  }
  printf("%d", count);
  return 0;
}