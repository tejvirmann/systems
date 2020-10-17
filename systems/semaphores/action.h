/*
 * Project: prodcom
 * File: action.h
 * All Files: main.c, queue_functions.c, action.c, queue_functions.h, 
 * queue_struct.h, action.h 
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This file serves as the method header for the action.c file. This 
 * file is meant to modulate the code and make everything easy and clean
 * to access.
 */

/*
    Reads in lines and adds then to queue
    Queue param needed to determine which queues to read to. 
*/
void* reader (void* one);
/*
    Goes through lines and edits them.
    Queue param need to determine which queue to munch to
    and which queue to read to. 
*/
void*  munch_1 (void*one_two);
/*
    Goes through lines and edits them.
    Queue param need to determine which queue to munch to
    and which queue to read to. 
*/
void*  munch_2(void*two_three);
/*
    Prints out the strings in the final 
    Queue and prints out the total number of strings
    Queue param needed to determine the final queue. 
*/
void*  writer(void *two_three);