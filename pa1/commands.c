/**
 * Implements command labels and their corresponding functions.
 *
 * @author Ted Mader
 * @date 2016-09-18
 */

#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char** mash_env_vars;
char** mash_env_vals;

char* command_labels[] = {
  "cd",
  "pwd",
  "setenv",
  "exit",
  NULL
};

int (*command_functions[]) (char**) = {
  &mash_cd,
  &mash_pwd,
  &mash_setenv,
  &mash_exit,
  NULL
};

void update_mash_env() {
  FILE* file = fopen("mash_env", "r");
  if (file == NULL) {
    perror("mash: env: error opening mash_env");
  }
  char** mash_env_vars = malloc(256 * sizeof(char*));
  char** mash_env_vals = malloc(256 * sizeof(char*));
  char line[256];
  int i = 0;
  while (fgets(line, sizeof(line), file)) {
    mash_env_vars[i] = strtok(line, "=");
    mash_env_vals[i] = strtok(NULL, "=");
    i++;
  }
  fclose(file);
  mash_env_vars[i] = NULL;
  mash_env_vals[i] = NULL;
}

int mash_cd(char** argv) {
  if (argv[1] == NULL) {
    chdir("~");
  } else {
    if (chdir(argv[1]) != 0) {
      printf("mash: cd: no such file or directory: %s\n", argv[1]);
    }
  }
  return 1;
}

int mash_pwd(char** argv) {
   char dir[1024];
   if (getcwd(dir, sizeof(dir)) != NULL)
       fprintf(stdout, "mash: current working dir: %s\n", dir);
   else
       perror("mash");
  return 1;
}

int mash_setenv(char** argv) {
  if (argv[1] == NULL) {
    return 1;
  }
  FILE* file = fopen("mash_env", "a");
  if (file == NULL) {
    printf("mash: setenv: error opening mash_env");
  } else {
    fprintf(file, "%s\n", argv[1]);
    fclose(file);
  }
  update_mash_env();
  return 1;
}

int mash_exit(char** argv) {
  return 0;
}
