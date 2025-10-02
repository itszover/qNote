#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

int find_last_page()
{
  int last_page = 0;

#ifdef _WIN32
  WIN32_FIND_DATA findFileData;
  HANDLE hFind = FindFirstFile("p*.txt", &findFileData);

  if (hFind == INVALID_HANDLE_VALUE)
  {
    return 0; // nenhum arquivo encontrado
  }

  do
  {
    int num;
    if (sscanf(findFileData.cFileName, "p%d.txt", &num) == 1)
    {
      if (num > last_page)
        last_page = num;
    }
  } while (FindNextFile(hFind, &findFileData));

  FindClose(hFind);

#else
  DIR *d = opendir(".");

  struct dirent *entry;
  while ((entry = readdir(d)) != NULL)
  {
    int num;
    if (sscanf(entry->d_name, "p%d.txt", &num) == 1)
    {
      if (num > last_page)
        last_page = num;
    }
  }

  closedir(d);
#endif

  return last_page;
}

void create_new_page(int last_page)
{
  char filename[7];
  sprintf(filename, "p%d.txt", last_page + 1);

  FILE *f = fopen(filename, "w");
}

void read_page(int page_number)
{
  char filename[7];
  sprintf(filename, "p%d.txt", page_number);

  FILE *f = fopen(filename, "r");

  if (f == NULL)
  {
    perror("Error opening file");
    return;
  }

  int c;
  while ((c = fgetc(f)) != EOF)
  {
    putchar(c);
  }
}

void write_page(int page_number)
{
  char filename[7];
  sprintf(filename, "p%d.txt", page_number);
  printf(">>>\n");

  char buffer[256];
  fgets(buffer, sizeof(buffer), stdin);
  FILE *j = fopen(filename, "a");

  fprintf(j, "%s\n", buffer);
  fclose(j);
}

int main(int argc, char **argv)
{
  int lp = find_last_page();

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--read") == 0)
    {
      read_page(lp);
      exit(EXIT_SUCCESS);
    }

    if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--new") == 0)
    {
      create_new_page(lp);
      exit(EXIT_SUCCESS);
    }
  }
  write_page(lp);

  return EXIT_SUCCESS;
}
