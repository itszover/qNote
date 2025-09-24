#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char *readline(void) {
  size_t cap = 16;
  size_t length = 0;
  char *buffer = malloc(cap);

  if (buffer == NULL) return NULL;

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

int main() {
  printf(">>>\n");
  char *line = readline();
  printf("%s", line);
}
