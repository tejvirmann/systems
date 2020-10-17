/*
 * Project: prodcom
 * File: action.c
 * All Files: main.c, queue_functions.c, action.c, queue_functions.h, 
 * queue_struct.h, action.h 
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This is file responsible for implementing queue manipulation functions
 * such as read into queue, edit queue strings, and print out the queue.
 */


#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "queue_struct.h"
#include "queue_functions.h"
#include <pthread.h>
#include <ctype.h>

/*
Reads a file from stdin. enqueues the lines. If line too long. print error, and chop.
Reads in lines and adds then to queue
Queue param needed to determine which queues to read to. 
*/
void* reader (void* queue_one){
    
    Queue *one; //create queue
    one = (Queue*) queue_one;

    size_t size = 4098;
    char *temp;
    char *str;

    int r;
    int len;
    int string_num;

    while (1){
        r = getline(&temp, &size, stdin); //scans file until EOF
    
        if(r == EOF){
           break;
        }

        string_num++;//keeps track of string if error
        len = strlen(temp); //str length

        if(len >= size){ 
            printf("Error: String %d is too long!", string_num);
            str = malloc(size*sizeof(char)); //edit temp
            memcpy(str, &temp[0], size-2);
            str[size-1] = '\0';
        }
        else{
            str = malloc(size*sizeof(char));
            strcpy(str, temp);
        }
        
        printf("reader: %s\n", str);
        EnqueueString(one, str);
    }

    EnqueueString(one, one->marker);
   
    pthread_exit(NULL);
}

/*
Runs through queues, if spaces (not tabs of \0) then replace with *
Goes through lines and edits them.
Queue param need to determine which queue to munch to
and which queue to read to. 
*/
void* munch_1(void* queue_one_two){
    
    Queue** one_two;
    one_two = (Queue**) queue_one_two;
    
    while(1){
        char *temp = DequeueString(one_two[0]);  
        //printf("munch_1: %s\n", temp);

        if(strcmp(marker,temp)==0){ //if the end of queue is reached
            break;
        }

        int len = strlen(temp); //length of string
        for(int i = 0; i < len; i++){
            if(temp[i] == ' '){
                temp[i] = '*';
            }
        }

        EnqueueString(one_two[1], temp);
//  printf("First in two buff:%s\n", one_two[1]->buffers[0]);
    }

    EnqueueString(one_two[1], one_two[1]->marker); //adding another null marker
   // printf("Calling munch_1 exit\n");
    pthread_exit(NULL);
}


/*
If lowercase, then change to uppercase.
Goes through lines and edits them.
Queue param need to determine which queue to munch to
and which queue to read to. 
*/
void* munch_2(void* queue_two_three){
    
    Queue** two_three;
    two_three = (Queue**) queue_two_three; //contains 2 queues

    while(1){
        char *temp = DequeueString(two_three[0]); 
//  printf("1st in two buff:%s\n", two_three[0]->buffers[0]);
//        char str[4098];
  //      strcpy(str,temp);
        printf("munch_2: %s\n", temp);

        if(strcmp(marker,temp)==0){ //if the end of queue is reached
            break;
        }

        int len = strlen(temp); //length of string

        for(int i = 0; i < len; i++){
            if(isalpha(temp[i])){  //is letter ?
                if(!(isupper(temp[i]))){  //is upper ?
                    temp[i] = toupper(temp[i]); //uppercase it
                }
            }
        }
        EnqueueString(two_three[1], temp);
    }

    EnqueueString(two_three[1], two_three[1]->marker); //adding NULL marker
    printf("Calling munch_2 exit\n");
    pthread_exit(NULL); //exit thread
} 

/*
Prints out the strings in the final 
Queue and prints out the total number of strings
Queue param needed to determine the final queue.
*/
void* writer(void* queue_three){
    
    Queue *three;
    three = (Queue*) queue_three; //contains 1 queue, queue three

    while(1){
        char *str = DequeueString(three);
        printf("writer: %s\n", str);

        if(strcmp(three->marker,str)==0){ //if the end of queue is reached
            break;
        }
        printf("%s\n", str);
        free(str);
        three->total_output++; //increments to get total strings printed
    }
    
    printf("%d\n",three->total_output); //should print total # of strings printed to stdout
    
    //printf("Calling writer exit\n");
    pthread_exit(NULL);
    
}