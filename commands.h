/**
 * Defines command labels and their corresponding functions.
 *
 * @author Ted Mader
 * @date 2016-09-17
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

int command_cd(char** argv);
int command_exit(char** argv);

char* command_labels[] = {
  "cd",
  "exit"
};

int (*command_functions[]) (char**) = {
  &command_cd,
  &command_exit
};

#endif
