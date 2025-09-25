#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *readline(void) {
  size_t cap = 16;
  size_t length = 0;
  char *buffer = malloc(cap);
  int c;

  while ((c = getchar()) != EOF && c != '\n') {
    if (length + 1 >= cap) {
      if (cap > SIZE_MAX / 2) {
        free(buffer);
        return NULL;
      }
      // double the memory cap
      buffer = realloc(buffer, cap *= 2);
    }
    buffer[length++] = (char)c;
  }

  if (c == EOF && length == 0) {
    free(buffer);
    return NULL;
  }

  buffer[length] = '\0';
  return buffer;
}

int main(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--read") == 0) {
      FILE *f = fopen("output.txt", "r");
      int c;
      while ((c = fgetc(f)) != EOF) {
        putchar(c);
      }
      exit(EXIT_SUCCESS);
    }
  }

  printf(">>>\n");

  char *line = readline();
  FILE *f = fopen("output.txt", "a");

  fprintf(f, "%s\n", line);
  fclose(f);
}
