/*
Project: 537ps
File: read_proc.c
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file is used to retrieve the info from the proc system
and to print the data afterwards. 
*/

//libs and .h's
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "parse_stat.h"
#include "parse_statm.h"
#include "parse_c.h"

/*
This method is to take the flags and output information
that the user wanted. This can only be done after retrieving 
the data from the proc system, which is what this file does first. 
param: flags int array.
output: int variable -1 if fail. and prints info.
*/
int read_proc(int *flags) { //used to be **flags

    char *s; //init
    char *S; 
    char *v; 
    char *c; 
    char *U; 
    const int sizer = 1000;
    const int fifty = 50;
    const int two = 2;

    //alloc
    s = (char*) malloc(fifty*sizeof(char));
    v = (char*) malloc(fifty*sizeof(char));
    S = (char*) malloc(fifty*sizeof(char));
    U = (char*) malloc(fifty*sizeof(char));
    c = (char*) malloc(sizer*sizeof(char));
    if((s || v || S || U ||c) == NULL){
        perror("Error: Memory Allocation error: reading");
        return -1;
    }

    s[0] ='\0'; // assign empty before. will 
    v[0] ='\0'; //be reassigned if needed.
    S[0] ='\0';
    U[0] ='\0';
    c[0] ='\0';

    if(flags[1] != -1){//stat get state
        s = parse_stat("s", flags[0]);  //flags[0] contains PID
        if(!strcmp(s,"-1")){
            perror("Error: File issue: retrieving s");
            return -1;
        }
    }
    if(flags[3] != -1){//stat
        S = parse_stat("S", flags[0]);
        if(!strcmp(S,"-1")){
            perror("Error: File issue: retrieving S");
            return -1;
        }
    }
    if(flags[4] != -1){//statm file
        v = parse_statm(flags[0]);
        if(!strcmp(v,"-1")){
            perror("Error: File issue: retrieving v");
            return -1;
        }
    }
    if(flags[2] != -1){//stat
        U = parse_stat("U", flags[0]);
        if(!strcmp(U,"-1")){
            perror("Error: File issue: retrieving U");
            return -1;
        }
    }
    if(flags[5] != -1){//cmdlne file
        c = parse_c(flags[0]);
        if('\0' == c[0]){
            perror("Error: File issue: retrieving c");
            return -1;
        }
        if(!strcmp(c,"-1")){
            perror("Error: File issue: retrieving c");
            return -1;
        }
    }

    //now print given information. 
    printf("%d:", flags[0]);
    if((flags[1] == 1) && strcmp(s,"")){ //s
        printf("state: %s", s);
    }
    if(flags[2] == 1){ //U
        printf(" Utime:%s", U);
    }
    if((flags[3] == 1) && strcmp(S,"")){ //S
        printf(" Stime:%s", S);
    }
    if((flags[4] == 1) && strcmp(v,"")){ //v
        printf(" Vmem:%s", v);
    }
    if(flags[5] == 1){ //c
        printf(" [%s]", c);
    }
    printf("\n");

    return 0;

}