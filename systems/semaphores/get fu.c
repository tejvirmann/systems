/*
 * Project: prodcom
 * File: queue_functions.c
 * All Files: main.c, queue_functions.c, action.c, queue_functions.h, 
 * queue_struct.h, action.h 
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This is file responsible for implementing queue functions such as enqueue, dequeue
 * createqueue, and printqueuestats. 
 */

#include <semaphore.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue_struct.h"

//init the sems
sem_t full;
sem_t empty;
sem_t mutex;

/*
    Creates a queue and returns its pointer. Given size. 
    More in header file. 
*/
Queue *CreateStringQueue(int size){
    Queue *q = malloc(sizeof(Queue)); 
    q->enqueueTime = 0;
    q->dequeueTime = 0;
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->totenqCount = 0;
    q->totdeqCount = 0;
    q->total_output = 0; //for writer
    q->size = size;
    q->buffers = malloc(size*sizeof(char*));
    sem_init(&full, 0, 0); 
    sem_init(&empty, 0, size); 
    sem_init(&mutex, 0, 1);
    return q;
}

/*
    Adds a string to the end of the queue. 
    More in header file. 
*/
void EnqueueString(Queue *q, char *string){
    sem_wait(&empty);//locks    
    sem_wait(&mutex);

    printf("In Enqueue:\n");

    clock_t t = clock();

    int loc = (q->enqueueCount + q->dequeueCount) % q->size; //gets index loc

    printf("Enq count: %d\n", q->enqueueCount);
    printf("Deq count:%d\n", q->dequeueCount);
    printf("Index: %d\n", loc);
   // printf("%s\n",string);
    q->buffers[loc] = string; //puts buffer in queue.
    printf("The String: %s\n", q->buffers[loc]);
    q->enqueueCount++; //to help find index
    q->totenqCount++; //total enqueue count
    printf("Enq Done\n");
    printf("\n");
    t = clock() -t;
    int time_taken = (t)/CLOCKS_PER_SEC;
    q->enqueueTime = q->enqueueTime + time_taken; //gets time.

    sem_post(&mutex);
    sem_post(&full);    
}

/*
    Removes a string from front of queue.
    More in header file. 
*/
char * DequeueString(Queue *q){
    sem_wait(&full);
    sem_wait(&mutex);

    printf("In Dequeue:\n");
    clock_t t = clock();
    
    char * removed;
   
    int loc = q->dequeueCount % q->size;
    printf("Enq count:%d\n", q->enqueueCount);
    printf("Deq count:%d\n", q->dequeueCount);
    printf("Index:%d\n", loc);
    printf("The String:%s\n", q->buffers[loc]);

    removed = q->buffers[loc];
    //q->buffers[loc] = NULL; //dont really even need this

    q->dequeueCount+=1;
    q->enqueueCount--;

    t = clock() - t; //get time diff
    int time_taken = (t)/CLOCKS_PER_SEC; //get diff in sec.
    q->dequeueTime = q->dequeueTime + time_taken;
    printf("Deq Done\n");
    printf("\n");
    
    sem_post(&mutex);
    sem_post(&empty);
    return removed;
}

/*
    Prints the queue info.
    More in header file. 
*/
void PrintQueueStats(Queue *q){
    int a = q->totenqCount;
    int b = q->totdeqCount;
    int c = q->enqueueTime;
    int d = q->dequeueTime;
    printf("Enqueue Count:%d , Dequeue Count:%d\n , Enqueue Time:%d , Dequeue Time:%d\n", a, b, c, d);
}