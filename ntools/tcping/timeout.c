/*******************************************************
 * timeout                                             *
 * Program to demonstrate a timeout routine            *
 *******************************************************/

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

/*
 * The environment for setjmp.
 */
jmp_buf env;

main()
{
  int i;
  char buf[160];
  extern void timeout();

  /*
   * Inform the system we want to catch the 
   * alarm signal 
   */ 
  signal(SIGALRM, timeout);

  /*
   * The code inside the if gets executed the first 
   * time through setjmp, the code inside the else
   * the second time.
   */
  if (setjmp(env) == 0) {
    /*
     * Issue a request for an alarm to be delivered 
     * in 5 seconds. alarm() is a system call.
     */
    alarm(5);

    /* 
     * Prompt for input. Waits for it forever. If
     * it's still waiting and an alarm signal comes, 
     * we come back to the setjmp() and never get
     * any further.
     */
    printf("Type a word; if you don't in 5 ");
    printf("seconds I'll use 'WORD' instead:");
    (void) fgets(buf, sizeof(buf), stdin);
    /* 
     * Turn off the alarm
     */
    alarm(0);
  }
  else {
    /*
     * Assume the default.
     */
    strcpy(buf, "WORD");
  }

  printf("\n The word is: %s\n", buf);
  exit(0);
}

/*
 * timeout -- catch the signal
 */
void timeout(sig)
int sig;
{
  int i;

  /*
   * Ignore the signal for the duration of this
   * routine.
   */
  signal(sig, SIG_IGN);
  
  /* 
   * We would perform any timeout-related 
   * functions here.
   */
  printf("...Timeout!\n");

  /*
   * Restore the action of the alarm signal
   */
  signal(SIGALRM, timeout);

  /* 
   * Return to the main routine at setjmp,
   * and make setjmp return 1.
   */
  longjmp(env, 1);
}

