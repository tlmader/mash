/**
 * Implements command labels and their corresponding functions.
 *
 * @author Ted Mader
 * @date 2016-09-18
 */

#include "commands.h"
#include <stdio.h>
#include <unistd.h>

char* command_labels[] = {
  "cd",
  "exit",
  NULL
};

int (*command_functions[]) (char**) = {
  &command_cd,
  &command_exit,
  NULL
};

int command_cd(char** argv) {
  if (argv[1] == NULL) {
    chdir("~");
  } else {
    if (chdir(argv[1]) != 0) {
      printf("cd: no such file or directory: %s\n", argv[1]);
    }
  }
  return 1;
}

int command_exit(char** argv) {
  return 1;
}
