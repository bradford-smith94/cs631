/* Bradford Smith (bsmith8)
 * CS 631 sish sish.h
 * 12/17/2016
 */

#ifndef _SISH_H_
#define _SISH_H_

#include <sys/types.h> /* for definition of pid_t */

#define BG_STR "&"
#define CD "cd"
#define CURRENT_PID_STR "$$"
#define ECHO "echo"
#define ENV_HOME "HOME"
#define ENV_SHELL "SHELL"
#define EXIT "exit"
#define EXIT_NO_EXEC 127
#define EXIT_STATUS_STR "$?"
#define IO_IN "<"
#define IO_OUT ">"
#define IO_OUT_APPEND ">>"
#define NAME "sish"
#define PROMPT_STR "sish$ "

/* global variables */
pid_t gl_current_pid;
pid_t* gl_bg_pids;
unsigned char gl_exit_code;
char gl_trace;
char* gl_home_path;
int gl_num_bg_pids;
int gl_saved_stdin;
int gl_saved_stdout;

/* functions */
int main(int, char**);
int parseBackground(char**);

char*** tokenizePipeline(char*);

void executeCommand(char**, int, int);
void executePipeline(char***);
void freePipeline(char***);
void handleSigInt();
void init();
void redirectIO(char**);
void removeIndex(char**, int);
void restoreIO();

#endif /* _SISH_H_ */
