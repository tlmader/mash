/**
 * Contains the main and looping functions.
 *
 * @author Ted Mader
 * @date 2016-09-14
 */

#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DELIMS " \t\r\n\a"

/**
 * Reads and returns an entire line from stream.
 *
 * @return the line
 */
char* get_input() {
  char* line = NULL;
  unsigned long bufsize = 0;
  getline(&line, &bufsize, stdin);
  size_t len = strlen(line);
  if (line[len - 1] == '\n') {
    line[len - 1] = '\0';
  }
  return line;
}

/**
* Modifies a string by breaking into tokens.
*
* @param line a string to be split
* @return the tokens
*/
char** split(char* line) {
  int bufsize = 64;
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token = strtok(line, DELIMS);
  int i = 0;
  while (token != NULL) {
    tokens[i] = token;
    i++;
    token = strtok(NULL, DELIMS);
  }
  return tokens;
}

/**
* Forks and executes a process.
*
* @param argv the vector of arguments
* @return the status
*/
int run(char** argv) {
  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) {
    execvp(*argv, argv);
    perror("mash: Error executing");
    exit(1);
  } else if (pid < 0) {
    perror("mash: Error forking");
    exit(1);
  } else {
    while (wait(&status) != pid);
  }
  return 1;
}

/**
* Executes a command if recognized, otherwise enters run().
*
* @param argv the vector of arguments
* @return the status returned by run()
*/
int execute(char** argv) {
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
 */
void loop() {
  char* line;
  char** argv;
  int status;
  do {
    printf("mash > ");
    line = get_input();
    argv = split(line);
    status = execute(argv);
    free(line);
    free(argv);
  } while (status);
}

/**
 * Called at program startup.
 *
 * @param argc the number of arguments
 * @param argv the vector of arguments
 */
int main(int argc, char** argv) {
  loop();

  return 0;
}
