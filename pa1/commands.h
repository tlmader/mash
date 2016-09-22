/**
 * Defines arrays and functions for commands.
 *
 * @author Ted Mader
 * @date 2016-09-17
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

extern char* command_labels[];
extern int (*command_functions[]) (char**);
extern char** mash_env_vars;
extern char** mash_env_vals;

void test_mash_env();
void update_mash_env();

int mash_cd(char** argv);
int mash_pwd(char** argv);
int mash_getenv(char** argv);
int mash_setenv(char** argv);
int mash_exit(char** argv);
#endif
