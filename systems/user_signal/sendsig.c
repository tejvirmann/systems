////////////////////////////////////////////////////////////////////////////////
// 
// File:        sendsig.c
// Other Files:      intdate.c, division.c
// Semester:         CS 354 Spring 2020
//
// Author:           Tejvir Mann
// Email:            tsmann@wisc.edu
// CS Login:         tejvir
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>


/* This program allows you to send a SIGUSR1 or
*  SIGINT sig to another program given the pid.
*/
int main(int argN, char *argV[]) {
    if (argN != 3) { //if wrong args 
        printf("Usage: <signal type> <pid>\n");
        return -1;
    }

    if (strlen(argV[1]) != 2) { //if sig not right length
        printf("Usage: <signal type> <pid>\n"); //print message
        return -1;
    }

    int pid = atoi(argV[2]); //get PID from CMLs
    char sig = argV[1][1]; //get desired sig type

    if (sig == 'i') { //if interrupt
        int success = kill(pid, SIGINT); //kills process and gets val
        if (success) { //if not 0
            printf("ERROR sending signal: SIGINT\n"); //message
        }
    } else if (sig == 'u') { //if signal desired is user sig 
        int success = kill(pid, SIGUSR1); //kills process and gets val
        if (success) { //if return val is not 0
            printf("ERROR sending signal: SIGUSR1\n"); //print message
        }
    } else { //if sig flag not above
        printf("Usage: <signal type> <pid>\n"); //print return
        return -1;
    }

    return 0; //if it all works
}