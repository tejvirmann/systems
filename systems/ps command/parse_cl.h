/*
Project: 537ps
File: parse_cl.h
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file is used by parse_cl.c in order to check if the arguments
entered onto the command line are are correct or invalid. 
*/

//libs
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

/*
This is the method used to check if the optarg for a certain
argument is valid.
param: char optarg
output: int 0, -1, or 1.
*/
int checker(char *optarg){
	if(NULL == optarg){ //then optarg empty, and on. 
		return 1;
	} 
	else if(!strcmp(optarg,"-")){ //then flag turns off. 
		return -1; 				//and do not display information about it. 
	}
	else if(!strcmp(optarg,"")){
		return -1;
	}
	else{
	    perror("Error: invalid command line!"); //anything else would be invalid.
	    return 0;
	}
}
