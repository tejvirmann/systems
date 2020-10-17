/*
Project: 537ps
File: parse_c.h
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file is called by read_proc to access the cmdline file 
for c, after info is retrieved it is returned and printed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

/*
This file is called by read_proc to access the cmdline file 
for c, after info is retrieved it is returned and printed.
param: int pid to get proc
output: string containing cmdline
*/
char* parse_c(int pid){
  //init
  const int sizer = 1000;
  const int two = 2;
  char *buffer;
  char filename[1000];
  char *empty; 
  char *ret;

  //alloc
  ret = (char*) malloc(two*sizeof(char));
  ret = "-1"; //if errors return this

  buffer = (char*) malloc(sizer*sizeof(char));
  empty = (char*) malloc(two*sizeof(char));
  if((buffer || empty) == NULL){
    return ret;
  }

  empty[0] ='\0'; //assign for if empty

  //open file
  sprintf(filename, "/proc/%d/cmdline", pid);
  FILE *stream;
  stream = fopen(filename, "r");
  if(NULL == stream){ //check if null
      return ret;
  }

  rewind(stream); //puts it at the beginning
  fread(buffer, sizeof(char), 4096, stream);  //reads file into buffer
  fclose(stream);

  //fix the buffer 
  int i = 0;
  int cont = 1;
  while(cont){
    if(buffer[i] == '\0'){ 
      if(buffer[i+1] == '\0'){ //then at end, stop.
        break;
      }
      buffer[i] = ' '; //replaces with spaces
    }
    i++;
  }

  if(!strcmp(empty,buffer)){ //if empty return empty.
    return empty;
  }
  else{
    return buffer;
  }
}