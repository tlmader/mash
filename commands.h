/**
 * Defines arrays and functions for commands.
 *
 * @author Ted Mader
 * @date 2016-09-17
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

int mash_cd(char** argv);
int mash_getenv(char** argv);
int mash_setenv(char** argv);
int mash_exit(char** argv);

extern char* command_labels[];
extern int (*command_functions[]) (char**);

#endif
