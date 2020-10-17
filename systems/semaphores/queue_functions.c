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

/*
    Creates a queue and returns its pointer. Given size. 
    More in header file. 
*/
Queue *CreateStringQueue(int size){
    Queue *q = malloc(sizeof(Queue)); 
    q->enqueueTime = 0;
    q->dequeueTime = 0;
    q->length = 0;
    q->start = 0;
    q->totenqCount = 0;
    q->totdeqCount = 0;
    q->total_output = 0; //for writer
    q->size = size;
    q->buffers = malloc(size*sizeof(char*));
    q->marker = malloc(10*sizeof(char)); 
    q->marker = "Ryann";
    sem_init(&(q->full), 0, 0); 
    sem_init(&(q->empty), 0, size); 
    sem_init(&(q->mutex), 0, 1);
    return q;
}

/*
    Adds a string to the end of the queue. 
    More in header file. 
*/
void EnqueueString(Queue *q, char *string){
   // printf("Top of Enqueue\n");   
    sem_wait(&(q->empty));//locks    
    sem_wait(&(q->mutex));
// printf("inside enqueue\n");
    clock_t t = clock();
    int loc = (q->length + q->start) % q->size; //gets index loc
    // printf("%d\n", q->length);
    // printf("%d\n", q->start);
 //    printf("%s stored at index:%d\n", string, loc);
   // printf("%s\n",string);
    q->buffers[loc] = string; //puts buffer in queue.
//     printf("String Enqd:%s\n", q->buffers[loc]);
    q->length++; //to help find index
    q->totenqCount++; //total enqueue count
// printf("enqueued\n");
    t = clock() -t;
    int time_taken = (t)/CLOCKS_PER_SEC;
    q->enqueueTime = q->enqueueTime + time_taken; //gets time.
  //  printf("total count after enqueue: %d\n", q->length);
    sem_post(&(q->mutex));
    sem_post(&(q->full));    
}

/*
    Removes a string from front of queue.
    More in header file. 
*/
char * DequeueString(Queue *q){
   // printf("Top of Dequeue\n");
    sem_wait(&(q->full));
    sem_wait(&(q->mutex));
    // printf("inside dequeue\n");
    clock_t t = clock();
    
    char * removed;
   
    int loc = q->start % q->size;
    // printf("%d\n", q->length);
    // printf("%d\n", q->start);
   // printf("%d\n", loc);
    // printf("hii\n");
   //  printf("String Deqed:%s\n", q->buffers[loc]);
    // printf("bye\n");
    removed = q->buffers[loc];
    //q->buffers[loc] = NULL; //dont really even need this
//printf("%s taken from index:%d\n", removed, loc);
    q->start+=1;
    q->length--;
//printf("total count after dequeue:%d\n", q->length);
    t = clock() - t; //get time diff
    int time_taken = (t)/CLOCKS_PER_SEC; //get diff in sec.
    q->dequeueTime = q->dequeueTime + time_taken;
    // printf("dequeued\n");
    sem_post(&(q->mutex));
    sem_post(&(q->empty));
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