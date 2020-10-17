/*
 * Project: prodcom
 * File: queue_functions.h
 * All Files: main.c, queue_functions.c, action.c, queue_functions.h, 
 * queue_struct.h, action.h 
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This file is the method header for queue_functions, it is meant to 
 * connect the struct and modulate the program. 
 */


#include "queue_struct.h"
/*
	Creates a queue and returns its pointer. Given size. 
	size param to create a queue of certain size.
*/
Queue *CreateStringQueue(int size);

/*
	Adds a string to the end of the queue. 
	Queue and string params to indicate which queue and 
	which string to add. 
*/
void EnqueueString(Queue *q, char *string);

/*
    removes a string to the end of the queue. 
    Queue param needed to determine which queue to dequeue.
*/
char * DequeueString(Queue *q);

/*
    Prints the Information
    Queue param needed to determine which queues stats
    to print. 
*/
void PrintQueueStats(Queue *q);