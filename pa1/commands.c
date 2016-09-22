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
  "getenv",
  "setenv",
  "refreshenv",
  "exit",
  NULL
};

int (*command_functions[]) (char**) = {
  &mash_cd,
  &mash_pwd,
  &mash_getenv,
  &mash_refreshenv,
  &mash_setenv,
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
  for (int j = 0; mash_env_vars[j] != NULL; j++) {
    printf("mash_env[%i]: %s=%s", j, mash_env_vars[j], mash_env_vals[j]);
  }
  return 1;
}

int mash_refreshenv() {
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
  char** mash_env_vars = malloc(256 * sizeof(char*));
  char** mash_env_vals = malloc(256 * sizeof(char*));
  char line[256];
  int i = 0;
  while (fgets(line, sizeof(line), file)) {
    // char* var = malloc(strlen(line) + 1);
    // strcpy(var, strtok(line, "="));
    // char* val = malloc(strlen(line) + 1);
    // strcpy(val, strtok(NULL, "="));
    mash_env_vars[i] = strdup(strtok(line, "="));
    mash_env_vals[i] = strdup(strtok(NULL, "="));
    i++;
  }
  fclose(file);
  mash_env_vars[i] = NULL;
  mash_env_vals[i] = NULL;
  for (int j = 0; mash_env_vars[j] != NULL; j++) {
    printf("mash_env[%i]: %s=%s", j, mash_env_vars[j], mash_env_vals[j]);
  }
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
  mash_refreshenv();
  return 1;
}

int mash_exit(char** argv) {
  return 0;
}
