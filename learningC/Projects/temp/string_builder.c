#include <stdio.h>

/*
 * Project: to create strings at runtime
 * Author: Kaustubh
 */

int main(int argc, char *argv[]) {
  printf("the game starts");
  for (int i = 0; i < 10; i++) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%s%d.txt", "hello", i);
    printf("buffer text is : %s\n", buffer);
  }
  return 0;
}