/*
Project: 537ps
File: parse_statm.h
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file is called by read_proc to access the statm file 
for v, after info is retrieved it is returned and printed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

/*
This file is called by read_proc to access the statm file 
for v, after info is retrieved it is returned and printed.
params: int pid
output: string info for v.
*/
 char* parse_statm(int pid){

 	//init
	const int sizer = 1000; //for alloc
	const int fifty = 50;
	const int two = 2;
	char filename[sizer];
	char *empty; //if empty
	char *v_string; //cast
	char *ret; 
	int v = 0; //to get
	ret = (char*) malloc(two*sizeof(char));
  	ret = "-1"; //if errors return this

	empty = (char*) malloc(two*sizeof(char));
	v_string = (char*) malloc(fifty*sizeof(char));
	if ('\0' == (empty[0] || v_string[0])){
		return ret;
	}
	
	empty[0] ='\0'; //if variable not present

	//open file
	sprintf(filename, "/proc/%d/statm", pid);
	FILE *f = fopen(filename, "r");
	if(NULL == f){ //check it.
		return ret;
	 }

	//read the file
	fscanf(f, "%d", &v); 
	fclose(f); 

	sprintf(v_string, "%d", v); //convert int to str

	if(0 == v){ //if empty don't show it.
		return empty;
	}
	else{
		return v_string; 
	}
}

