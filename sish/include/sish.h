/* Bradford Smith (bsmith8)
 * CS 631 sish sish.h
 * 12/14/2016
 */

#ifndef _SISH_H_
#define _SISH_H_

#define NAME "sish"
#define PROMPT_STR "sish$ "

unsigned char gl_exit_code;
char gl_trace;

int main(int, char**);

void init();
void handleSigInt();

#endif /* _SISH_H_ */
