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

extern char* mash_commands[];
extern int (*mash_functions[]) (char**);

/**
 * Returns an array of the environment variabless for mash.
 *
 * return the vars array
 */

extern char** mash_env_vars();
/**
 * Returns an array of the values of the environment variables for mash.
 *
 * return the vals array
 */
extern char** mash_env_vals();

/**
 * Navigates to the path defined by argv[1].
 *
 * @param argv an array of args
 * return 1 if successful
 */
int mash_cd(char** argv);

/**
 * Prints the current working directory of mash.
 *
 * return 1 if successful
 */
int mash_pwd(char** argv);

/**
 * Prints the environment variables recognized by mash.
 *
 * return 1 if successful
 */
int mash_getenv();

/**
 * Refreshes the environment variables recognized by mash.
 *
 * return 1 if successful
 */
int mash_refreshenv();

/**
 * Sets a new environment variable to be recognized by mash.
 *
 * @param argv an array of args
 * return 1 if successful
 */
int mash_setenv(char** argv);

/**
 * Exits the program.
 *
 * @param argv an array of args
 * return 1 if successful
 */
int mash_exit();

#endif
