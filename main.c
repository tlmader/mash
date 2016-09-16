/**
 * Contains the main and looping functions.
 *
 * @author Ted Mader
 * @date 2016-09-14
 */

#include <stdio.h>
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
  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

/**
* Modifies a string by breaking into tokens.
*
* @param line a string to be split
* @return the tokens
*/
char** split(char* line) {
  char** tokens;
  char* token = strtok(line, DELIMS);
  int i;
  while (token != NULL) {
    tokens[i] = token;
    i++;
    token = strtok(NULL, DELIMS);
  }
  return tokens;
}

int execute(char** argv) {
  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) {
    if (execvp(*argv, argv) < 0) {
      printf("mash: execute failed\n");
      exit(1);
    }
  } else if (pid < 0) {
    printf("mash: fork failed\n");
    exit(1);
  } else {
    while (wait(&status) != pid);
  }
}

/**
 * Handles commands during the shell loop
 */
void loop() {
  char* line;
  char** args;
  int status;
  do {
    printf("mash > ");
    line = get_input();
    args = split(line);
    status = execute(args);
    free(line);
    free(args);
  }
}

/**
* Called at program startup.
*
* @param argc the number of arguments
* @param argv the array of arguments
*/
int main(int argc, char** argv) {
  loop();

  return 0;
}
