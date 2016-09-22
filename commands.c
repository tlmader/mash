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

char** vars;
char** vals;

char** mash_env_vars() {
  return vars;
}

char** mash_env_vals() {
  return vals;
}

char* mash_commands[] = {
  "cd",
  "pwd",
  "getenv",
  "setenv",
  "refreshenv",
  "exit",
  NULL
};

int (*mash_functions[]) (char**) = {
  &mash_cd,
  &mash_pwd,
  &mash_getenv,
  &mash_setenv,
  &mash_refreshenv,
  &mash_exit,
  NULL
};

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

int mash_getenv() {
  for (int j = 0; vars[j] != NULL; j++) {
    printf("mash_env[%i]: %s=%s\n", j, vars[j], vals[j]);
  }
  return 1;
}

int mash_refreshenv() {
  vars = malloc(256 * sizeof(char*));
  vals = malloc(256 * sizeof(char*));
  FILE* file = fopen("mash_env", "r");
  if (file == NULL) {
    printf("mash: creating mash_env\n");
    file = fopen("mash_env", "w");
    if (file == NULL) {
      printf("mash: setenv: error creating mash_env\n");
      return 0;
    }
    fclose(file);
    return 1;
  }
  vars = malloc(256 * sizeof(char*));
  vals = malloc(256 * sizeof(char*));
  char line[256];
  int i = 0;
  while (fgets(line, sizeof(line), file)) {
    char* temp_var = strtok(line, "=");
    char* temp_val = strtok(NULL, "= \t\r\n\a");
    vars[i] = malloc(strlen(temp_var) + 1);
    vals[i] = malloc(strlen(temp_val) + 1);
    strcpy(vars[i], temp_var);
    strcpy(vals[i], temp_val);
    i++;
  }
  fclose(file);
  vars[i] = NULL;
  vals[i] = NULL;
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
  return mash_refreshenv();
}

int mash_exit() {
  return 0;
}
