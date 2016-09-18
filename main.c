/**
 * Contains the main and program logic for the mash shell.
 *
 * @author Ted Mader
 * @date 2016-09-14
 */

#include "commands.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Reads and returns an entire line from stream.
 *
 * @return the line
 */
char* get_input() {
  char* line = NULL;
  unsigned long bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

/**
 * Modifies a string by breaking into tokens.
 *
 * @param line a string to be split
 * @return the tokens
 */
char** split(char* line, char* delims) {
  int bufsize = 64;
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token = strtok(line, delims);
  int i = 0;
  while (token != NULL) {
    tokens[i] = token;
    i++;
    token = strtok(NULL, delims);
  }
  tokens[i] = NULL;
  return tokens;
}

/**
 * Handles stream redirection and returns the args for the command.
 *
 * @param argv the vector of args
 * @param the vector of args for the command
 */
char** redirect(char** argv) {
  int i = 0;
  int in = 0;
  int out = 0;
  int new_arg = 1;
  int bufsize = 64;
  char** new_argv = malloc(bufsize * sizeof(char**));
  while (argv[i] != NULL) {
    if (strcmp(argv[i], "<") == 0) {
      in = open(argv[i + 1], O_RDONLY);
      dup2(in, 0);
      close(in);
      new_arg = 0;
      new_argv[i] = NULL;
    } else if (strcmp(argv[i], ">") == 0) {
      out = open(argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
      dup2(out, 1);
      close(out);
      new_arg = 0;
      new_argv[i] = NULL;
    }
    if (new_arg) {
      new_argv[i] = argv[i];
    }
    i++;
  }
  return new_argv;
}

/**
* Forks and executes a process.
*
* @param argv the vector of args
* @return the status
*/
int run(char** argv) {
  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) {
    argv = redirect(argv);
    execvp(*argv, argv);
    printf("mash: command not found: %s\n", *argv);
    exit(1);
  } else if (pid < 0) {
    perror("mash");
    exit(1);
  } else {
    while (wait(&status) != pid);
  }
  return 1;
}

/**
* Executes a command if recognized, otherwise enters run().
*
* @param argv the vector of args
* @return the status returned by run()
*/
int execute(char** argv) {
  if (*argv == NULL) {
    return 1;
  }
  int i = 0;
  while (command_labels[i] != NULL) {
    if (strcmp(argv[0], command_labels[i]) == 0) {
      return (*command_functions[i])(argv);
    }
    i++;
  }
  return run(argv);
}

/**
 * Handles commands during the shell loop
 *
 * @return the status
 */
int loop() {
  char* line;
  char** tokens;
  int status = 1;
  do {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char* delims = "/";
    char* temp = strtok(cwd, delims);
    char* dir;
    while (temp != NULL) {
      dir = temp;
      temp = strtok(NULL, delims);
    }
    printf("-> %s ", dir);
    line = get_input();
    tokens = split(line, "|");
    int i = 0;
    while (tokens[i] != NULL && status) {
      status = execute(split(line, " \t\r\n\a"));
      i++;
    }
    free(line);
    free(tokens);
  } while (status);
  return status;
}

/**
 * Called at program startup.
 *
 * @param argc the number of args
 * @param argv the vector of args
 */
int main(int argc, char** argv) {
  return loop();
}
