/*
 * Project: prodcom
 * File: queue_struct.h
 * All Files: main.c, queue_functions.c, action.c, queue_functions.h, 
 * queue_struct.h, action.h 
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This file is the method header for the struct, the struct is meant to
 * be accessed by many parts of the code and is placed here for easy access.
 */

#ifndef QUEUE_H
#define QUEUE_H

/*
This is the Queue struct which is needed and called every time a Queue is created.
*/
typedef struct Queue {
    char** buffers;
    char* marker;
    int size;
    int enqueueCount; //always between 0-10
    int dequeueCount; //always between 0-10
    int totenqCount;
    int totdeqCount;
    int enqueueTime; // we cannot intitalize variables here. needs to be done in CreateStringQueue()
    int dequeueTime;
    int total_output;
} Queue;
                                                                              #endif
