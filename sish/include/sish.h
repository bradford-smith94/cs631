/* Bradford Smith (bsmith8)
 * CS 631 sish sish.h
 * 12/15/2016
 */

#ifndef _SISH_H_
#define _SISH_H_

#include <sys/types.h> /* for definition of pid_t */

#define CD "cd"
#define CURRENT_PID_STR "$$"
#define ECHO "echo"
#define ENV_HOME "HOME"
#define ENV_SHELL "SHELL"
#define EXIT "exit"
#define EXIT_STATUS_STR "$?"
#define NAME "sish"
#define PROMPT_STR "sish$ "

pid_t gl_current_pid;
unsigned char gl_exit_code;
char gl_trace;
char* gl_home_path;

int main(int, char**);

char*** tokenizePipeline(char*);

void executePipeline(char***);
void freePipeline(char***);
void init();
void handleSigInt();

#endif /* _SISH_H_ */
