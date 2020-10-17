////////////////////////////////////////////////////////////////////////////////
// 
// File:        intdate.c
// Other Files:      sendsig.c, division.c
// Semester:         CS 354 Spring 2020
//
// Author:           Tejvir Mann
// Email:            tsmann@wisc.edu
// CS Login:         tejvir
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int the_time = 3; //global variables for time and # of SIGUSR1 sigs
int numOfSIGG1 = 0;

/* SIGUSR1 handler. It recieves a user signal
*  and increases the total value of signals
*  sent by the user.
*/
void handler_SIGUSR1() {
    printf("SIGUSR1 caught!\n"); //message
    numOfSIGG1++; //inc total count
}

/* Here is handler for SIGALRM. This gets the current
*  date and time and then prints and the PID. It after sets 
*  another alarm for 3 seconds.
*/
void handler_SIGALRM() {
    time_t currTime; //declare time var
    time(&currTime);

    int pid = getpid(); //gets pid var
    printf("PID: %d | Current Time: %s", pid, ctime(&currTime)); //print pid
    alarm(the_time); //sets another alarm
}

/* Handler for SIGINT. When calledd
*  prints message and number
*  of user sigs recieved through the program. Then the
*  program will terminate.
*/
void handler_SIGINT() {
    printf("\nSIGINT received.\n"); //prints message and num
    printf("SIGUSR1 was recieved %d times. Exiting now.\n", numOfSIGG1);
    exit(0); //exits
}

int main() {
    struct sigaction sigalrm; //makes a new sigaction + clear to 0
    memset (&sigalrm, 0, sizeof(sigalrm));
    sigalrm.sa_handler = handler_SIGALRM; // update the sa_handler
    if (sigaction(SIGALRM, &sigalrm, NULL) != 0) { // bind the SIGALRM to handler
        printf("Error binding to signal: SIGALRM\n");
        return -1;
    }

    struct sigaction sigusr1; // create a new sigaction + clear
    memset (&sigusr1, 0, sizeof(sigusr1));
    sigusr1.sa_handler = handler_SIGUSR1; // update the sa_handler
    if (sigaction(SIGUSR1, &sigusr1, NULL) != 0) {  // bind the signal SIGUSR1 
        printf("Error binding to signal: SIGUSR1.\n");
        return -1;
    }

    struct sigaction sigint;  // create a new sigaction + clear
    memset (&sigint, 0, sizeof(sigint)); // update the sa_handler
    sigint.sa_handler = handler_SIGINT;
    if (sigaction(SIGINT, &sigint, NULL) != 0) { // bind the signal SIGINT
            printf("Error binding to signal: SIGINT.\n");
            return -1;
    }

    printf("Pid and time will be printed every 3 seconds.\n"); //print message
    printf("Enter ^C to end the program.\n");
    alarm(the_time); //set initial alarm

    while (1) {} //infinite loop: ACTIVATE
    return 0; //returns just because 
}