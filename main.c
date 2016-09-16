/**
 * Contains the main and looping functions.
 * @author Ted Mader
 * @date 2016-09-14
 */

/**
 * Handles commands during the shell's loop
 */
void loop() {
  char* line;
  char** args;
  int status;
  do {
    printf("mash > ");
    line = read();
    args = split(line);
    status = execute(args);
    free(line);
    free(args);
  } while(status);
}

/**
* Called at program startup.
*
* @param argc the number of arguments
* @param argv the array of arguments
*/
int main(int argc, char **argv) {
  loop();

  return 0;
}
