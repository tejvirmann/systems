/*
Project: 537ps
File: parse_stat.h
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file is called by read_proc to access the stat file 
for s, S and U. after info is retrieved it is returned and printed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>


/*
This file is called by read_proc to access the stat file 
for s, S and U. after info is retrieved it is returned and printed.
pararm: a string containing the letter, and a int with pid. 
output: a string containing the info based on the letter.
*/
char* parse_stat(char *letter, int pid){
  //init
  const int sizer = 1000; //for size alloc
  const int fifty = 50;
  const int two = 2; 
  long unsigned int uS = 0; //to get info
  long unsigned int uU = 0;
  int S = 0; //for casting
  int U = 0;
  char filename[sizer]; //for file
  char s = 'A'; //to get s
  char *empty; //if no data
  char *S_tostring; //in order to return string
  char *U_tostring;
  char *s_tostring;
  char *ret; //if error.

  //alloc
  ret = (char*) malloc(two*sizeof(char));
  ret = "-1"; //if errors return this
  empty = (char*) malloc(two*sizeof(char));
  S_tostring = (char*) malloc(fifty*sizeof(char));
  U_tostring = (char*) malloc(fifty*sizeof(char));
  s_tostring = (char*) malloc(fifty*sizeof(char));
  if ((s_tostring || U_tostring || S_tostring || empty) == 0){ //check if correct.  
    return ret; //error checker.
  }
  //assign
  empty[0] ='\0';
  s_tostring[0] = '0';

  sprintf(filename, "/proc/%d/stat", pid); //get file 
  FILE *f = fopen(filename, "r"); //open
  if(NULL == f){ //if fail
      return ret;
  }
  
  //open file get data.
  fscanf(f, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu",&s, &uS, &uU);
  fclose(f); 
  S = (int) uS; //unsigned converted to ints.
  U = (int) uU;
  sprintf(S_tostring, "%d", S); //ints converted to strings
  sprintf(U_tostring, "%d", U);
  s_tostring[0] = s; //char converted to string

  if(!(strcmp(letter,"s"))){ //checks s
      if(!(strcmp(s_tostring,"0"))){ //if empty 
        return empty;
      }
      else{
        return s_tostring;
      }
  }
  if(!(strcmp(letter,"S"))){ //checks S
      if(0 == S){ //if empty 
        return empty;
      }
      else{
        return S_tostring;  
      }
  }
  if(!(strcmp(letter,"U"))){ //checks U
    return U_tostring; 
  }

  return ret;
}
