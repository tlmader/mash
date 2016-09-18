/**
 * Defines arrays and functions for commands.
 *
 * @author Ted Mader
 * @date 2016-09-17
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

int command_cd(char** argv);
int command_exit(char** argv);

extern char* command_labels[];
extern int (*command_functions[]) (char**);

#endif
