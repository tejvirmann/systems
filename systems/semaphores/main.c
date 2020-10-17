/*
 * Project: prodcom
 * File: main.c
 * All Files: main.c, queue_functions.c, action.c, queue_functions.h, 
 * queue_struct.h, action.h 
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This is the main file responsible for using all the methods within other files to 
 * make the pthreads and to create the queues.
 */

#include <semaphore.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "queue_functions.h"
#include "action.h"
#include <pthread.h>

/*
This method controls and manipulates the other methods and files in order to take 
in the input, put it through queues, manipulate it in a uniue way, then finally
print the strings.
*/
int main() {

    //create 3 queues 
    int size = 11; //leave space for the 1 null marker. 
    Queue *one = CreateStringQueue(size);    
    Queue *two = CreateStringQueue(size);
    Queue *three = CreateStringQueue(size);

    //queue pointers. to help transfer info. 
    Queue **one_two = malloc(2*sizeof(Queue*)); 
    one_two[0] = one;
    one_two[1] = two;
    
    Queue **two_three = malloc(2*sizeof(Queue*));
    two_three[0] = two;    
    two_three[1] = three;

    //declare
    pthread_t th1;
    pthread_t th2;
    pthread_t th3;
    pthread_t th4;
    
    //create threads
    int iret1 = pthread_create( &th1, NULL, reader, (void*) one);
    int iret2 = pthread_create( &th2, NULL, munch_1, (void*)one_two);
    int iret3 = pthread_create( &th3, NULL, munch_2, (void*) two_three);
    int iret4 = pthread_create( &th4, NULL, writer, (void*)three);
    if((iret1 || iret2 || iret3 || iret4) != 0){ //check threads
       perror("Error: thread creation issue.");
        return 1;
    }

    //catch exited threads
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);

    //print queue stats. 
    PrintQueueStats(one);
    PrintQueueStats(two);
    PrintQueueStats(three);

    return 1;
}