////////////////////////////////////////////////////////////////////////////////
// 
// File:        division.c
// Other Files:      sendsig.c, intdate.c
// Semester:         CS 354 Spring 2020
//
// Author:           Tejvir Mann
// Email:            tsmann@wisc.edu
// CS Login:         tejvir
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global var for # of div operations
int divN = 0;

/* Handler method for SIGINT.Prints the 
* # of succesful ops, and terminates 
*  the program.
*/
void handler_SIGINT() {
    printf("\nTotal number of operations successfully completed: %d\n", divN);
    printf("The program will be terminated.\n");
    exit(0); 
}

/* Handler method for SIGFPE. Prints an 
*  error message to the console, # of succesful operations,
*  then terminates prog.
*/
void handler_SIGFPE() {
    printf("Error: a division by 0 operation was attempted.");
    printf("\nTotal number of operations completed successfully: %d\n", divN);
    printf("The program will be terminated.\n");
    exit(0);
}

/* Sets up the handlers and
*  binds them to the correct sigs. Takes input
*  preforms div.
*/
int main() {
    struct sigaction sigfpe; //gets sig structure
    memset (&sigfpe, 0, sizeof(sigfpe));
    sigfpe.sa_handler = handler_SIGFPE; //sets up handler
    if (sigaction(SIGFPE, &sigfpe, NULL) != 0) { //binds sig hand to SIGFPE
        printf("Error binding to signal: SIGFPE\n"); //if fail
        return -1;
    }


    struct sigaction sigint; //gets sig structure for SIGINT
    memset (&sigint, 0, sizeof(sigint));
    sigint.sa_handler = handler_SIGINT; //set up handler
    if (sigaction(SIGINT, &sigint, NULL) != 0) { //bind handler
        printf("Error binding to signal: SIGINT\n"); //if fail
        return -1;
    }

    int the_BUFF = 100; // create buffer var with desired buffer size
    char inputOne[the_BUFF];  //two char arrays of buffer length for input one and two
    char inputTwo[the_BUFF];
    int first, second; //inout variables
    int result, remainder; //result and reimainder vars

    while (1) {
        printf("Enter first integer: "); //prompt 1
        fgets(inputOne, the_BUFF, stdin); //get user input
        first = atoi(inputOne); //convert to int
        printf("Enter second integer: "); ///prompt 2
        fgets(inputTwo, the_BUFF, stdin);
        second = atoi(inputTwo); //math
        result = first / second;
        remainder = first % second;
        printf("%d / %d is %d with a remainder of %d\n", first, second, result, remainder); //results
        divN++; //inc
  }
  
  return 0; //for good measure 
}
