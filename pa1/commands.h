/**
 * Defines arrays and functions for commands.
 *
 * @author Ted Mader
 * @date 2016-09-17
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

char** vars;
char** vals;

extern char* command_labels[];
extern int (*command_functions[]) (char**);
extern char** mash_env_vars();
extern char** mash_env_vals();

int mash_cd(char** argv);
int mash_pwd(char** argv);
int mash_getenv();
int mash_refreshenv();
int mash_setenv(char** argv);
int mash_exit(char** argv);
#endif
