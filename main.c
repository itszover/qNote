#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *readline(void)
{
  size_t cap = 16;
  size_t length = 0;
  char *buffer = malloc(cap);
  int c;

  while ((c = getchar()) != EOF && c != '\n')
  {
    if (length + 1 >= cap)
    {
      if (cap > SIZE_MAX / 2)
      {
        free(buffer);
        return NULL;
      }
      // double the memory cap
      buffer = realloc(buffer, cap *= 2);
    }
    buffer[length++] = (char)c;
  }

  if (c == EOF && length == 0)
  {
    free(buffer);
    return NULL;
  }

  buffer[length] = '\0';
  return buffer;
}

int main(int argc, char **argv)
{
  FILE *f = fopen(".config", "r");
  char tmp[256];

  while (fgets(tmp, sizeof(tmp), f))
  {
    tmp[strcspn(tmp, "\n")] = '\0';

    char *space = strchr(tmp, ' ');
    size_t prop_len = space - tmp;
    size_t val_len = strlen(space + 1);
    char *property = malloc(prop_len + 1);
    char *value = malloc(val_len + 1);

    memcpy(property, tmp, prop_len);
    property[prop_len] = '\0';
    strcpy(value, space + 1);

    printf("Property: %s\n", property);
    printf("Value: %s\n", value);

    free(property);
    free(value);
  }

  fclose(f);
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--read") == 0)
    {
      FILE *f = fopen("p0.txt", "r");
      int c;
      while ((c = fgetc(f)) != EOF)
      {
        putchar(c);
      }
      exit(EXIT_SUCCESS);
    }

    if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--new") == 0)
    {
      char filename[7];

      for (int i = 0; i < 1000; i++)
      {
        sprintf(filename, "p%d.txt", i);
        FILE *f = fopen(filename, "r");
        if (f == NULL)
        {
          printf("Creating new file: %s\ni=%d\n", filename, i);
          sprintf(filename, "p%d.txt", i);
          FILE *s = fopen(filename, "w");
          printf(">>>\n");
          char *line = readline();
          fprintf(s, "%s\n", line);
          fclose(f);
          exit(EXIT_SUCCESS);
        }
        fclose(f);
      }
      exit(EXIT_SUCCESS);
    }
  }

  printf(">>>\n");

  char *line = readline();
  FILE *j = fopen("output.txt", "a");

  fprintf(j, "%s\n", line);
  fclose(j);
}
