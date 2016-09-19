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
 * @param delims a string of delimiters
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
 * @param argv a vector of args
 * @return the vector of args for the command
 */
int redirect(char** argv) {
  int i = 0;
  int in = 0;
  int out = 0;
  int add_to_command = 1;
  int bufsize = 64;
  char** command = malloc(bufsize * sizeof(char**));
  while (argv[i] != NULL) {
    if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], ">") == 0) {
      if (add_to_command) {
        add_to_command = 0;
        command[i] = NULL;
      }
      int status;
      pid_t pid = fork();
      if (pid == -1) {
        perror("mash");
        exit(1);
      } else if (pid == 0) {
        if (strcmp(argv[i], "<") == 0) {
          in = open(argv[i + 1], O_RDONLY);
          dup2(in, 0);
          close(in);
        } else if (strcmp(argv[i], ">") == 0) {
          out = open(argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
          dup2(out, 1);
          close(out);
        }
        execvp(*command, command);
        printf("mash: command not found: %s\n", *command);
        exit(1);
      } else {
        while (wait(&status) != pid);
      }
    }
    if (add_to_command) {
      command[i] = argv[i];
    }
    i++;
  }
  return 1;
}

/**
* Forks and executes a process.
*
* @param argv a vector of args
* @param use_redir a condition for using redirection
* @return the status
*/
int run(char** argv) {
  int status;
  pid_t pid = fork();
  if (pid == -1) {
    perror("mash");
    exit(1);
  } else if (pid == 0) {
    execvp(*argv, argv);
    printf("mash: command not found: %s\n", *argv);
    exit(1);
  } else {
    while (wait(&status) != pid);
  }
  return 1;
}

/**
 * Forks a child process using pipes.
 *
 * @param in a pipe input
 * @param out a pipe output
 * return the status
 */
void pipe_spawn_child(char** command, int input, int output) {
  int status;
  pid_t pid = fork();
  if (pid == -1) {
    perror("mash");
    exit(1);
  } else if (pid == 0) {
    if (input != 0) {
      dup2(input, 0);
      close(input);
    }
    if (output != 1) {
      dup2(output, 1);
      close(output);
    }
    execvp(*command, command);
  } else {
    while (wait(&status) != pid);
  }
}

/**
 * Pipes multiple commands.
 *
 * @param commands an array of commands
 * return the status
 */
int pipe_commands(char** commands) {
  int status;
  pid_t pid = fork();
  if (pid == -1) {
    perror("mash");
    exit(1);
  } else if (pid == 0) {
    int i = 0;
    int input = 0;
    int fd[2];
    while (commands[i + 1] != NULL) {
      pipe(fd);
      pipe_spawn_child(split(commands[i], " \t\r\n\a"), input, fd[1]);
      close(fd[1]);
      input = fd[0];
      i++;
    }
    if (input != 0) {
      dup2(input, 0);
    }
    char** last_command = split(commands[i], " \t\r\n\a");
    execvp(*last_command, last_command);
  } else {
    while (wait(&status) != pid);
  }
  return 1;
}

/**
* Checks for a mash command, otherwise enters run().
*
* @param argv the vector of args
* @return the status returned by run()
*/
int check_mash_commands(char** argv) {
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
  return 0;
}

/**
 * Handles commands during the shell loop
 *
 * @return the status
 */
int loop() {
  char* line;
  int status = 1;
  while (status) {
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
    char** argv = split(line, " \t\r\n\a");
    if (check_mash_commands(argv) == 0) {
      if ((strstr(line, "|") && strstr(line, "<")) || (strstr(line, "|") && strstr(line, ">"))) {
        printf("mash: cannot parse '|' with '<' or '>'\n");
      } else if (strstr(line, "|")) {
        status = pipe_commands(split(line, "|"));
      } else if (strstr(line, "<") || strstr(line, ">")) {
        status = redirect(argv);
      } else {
        status = run(argv);
      }
    }
    free(line);
    free(argv);
  }
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
