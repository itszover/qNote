#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

const size_t MAX_BUFFER_SIZE = 256;
const size_t MAX_FILENAME_SIZE = 32;
const char PAGE_FORMAT[] = "%d.txt";

int find_last_page()
{
  int last_page = 0;

#ifdef _WIN32
  WIN32_FIND_DATA findFileData;
  HANDLE hFind = FindFirstFile("*.txt", &findFileData);

  if (hFind == INVALID_HANDLE_VALUE)
  {
    return 0; // nenhum arquivo encontrado
  }

  do
  {
    int num;
    if (sscanf(findFileData.cFileName, PAGE_FORMAT, &num) == 1)
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
    if (sscanf(entry->d_name, PAGE_FORMAT, &num) == 1)
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
  char filename[MAX_FILENAME_SIZE];
  sprintf(filename, PAGE_FORMAT, last_page + 1);

  FILE *f = fopen(filename, "w");

  fclose(f);
}

void read_page(int page_number)
{
  char filename[MAX_FILENAME_SIZE];
  sprintf(filename, PAGE_FORMAT, page_number);

  FILE *f = fopen(filename, "r");

  int c;
  while ((c = fgetc(f)) != EOF)
  {
    putchar(c);
  }

  fclose(f);
}

void write_page(int page_number)
{
  char filename[MAX_FILENAME_SIZE];
  sprintf(filename, PAGE_FORMAT, page_number);

  FILE *f = fopen(filename, "a");
  
  printf("#\n");

  char buffer[MAX_BUFFER_SIZE];
  fgets(buffer, sizeof(buffer), stdin);
  
  fprintf(f, "%s\n", buffer);
  fclose(f);
}

int main(int argc, char **argv)
{
  int last_page = find_last_page();

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--read") == 0)
    {
      int input;

      if (i + 1 >= argc)
      {
        input = last_page;
      }
      else
      {
        input = atoi(argv[i + 1]);
      }

      int page = input > last_page ? last_page : input;

      read_page(page);
      
      return EXIT_SUCCESS;
    }

    if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--new") == 0)
    {
      create_new_page(last_page);

      return EXIT_SUCCESS;
    }
  }
  
  write_page(last_page);

  return EXIT_SUCCESS;
}
