/*
Project: 537ps
File: parse_cl.c
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file contains a method that checks the cmdline string and turns on and off certain
flags depending on data wanted. Further, this method also checks if info 
was entered into the cmdline correctly.
*/

//libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "parse_cl.h"

/*
This is the method that checks the cmdline string and turns on and off certain
flags depending on data wanted. Further, this method also checks if info 
was entered into the cmdline correctly.
params: argc - the # of argvs passed in. argv - the string from the cmdline.
pid_p: this is the pid of the proc to be checked. this is -1 initially. 
output: int array of flags which also includes the pid. 
*/
int *parse_cl(int argc, char **argv, int pid_p) {
     
    const int f_size = 7;
    int *flags;
    int option = 0; 
    int pid = -1; //pid
    int s_flag = -1; //flags
    int U_flag = -1;
    int S_flag = -1; 
    int v_flag= -1; 
    int c_flag = -1;
    int p_flag = -1;

    int *ret; //if error ret this.
    ret = malloc(sizeof(int)*f_size);
    ret[0] = -1;

    flags = malloc(sizeof(int)*f_size);
    if(NULL == flags){ //check malloc.
        perror("Error: memory allocation.");
        return ret;
    }

    if(0 == argc) { //arg checkers
        perror("Error: No Arguments");  
        flags[0] = -1;
        return flags;
    }
    if(NULL == argv) {
        perror("Error:c-line is Null");
        flags[0] = -1;
        return flags;
    }

    if(-1 != pid_p){ //if method used to get all PIDS.
        p_flag = 1; //in the case where no
        U_flag =1;  //pid is specified and 
        c_flag = 1;  //get_proc called beforehand to get user pids. 
        pid = pid_p;
    }
   
    while ((option = getopt(argc, argv, "p:s::S::v::U::c::")) != -1){ //parses argv
         if('?' == option){ //if wrong cmdline info.
         flags[0] = -1;
         return flags;
        }  
         switch(option){
                case 'p':
                    p_flag = 1; // if option is not present then display 
                    pid = atoi(optarg); //information for all processes of the current user
                    if (0 == pid && (strcmp(optarg,"0")!= 0)){ //checks pid.
                        perror("Error: PID is not integer");
                        flags[0] = -1;
                        return flags;
                    }
                    else{ //U and C on
                        U_flag = 1;
                        c_flag = 1;
                    }
                    break;
                case 's':
                    s_flag = checker(optarg);
                    if(0 == s_flag){ //check return
                        flags[0] = -1;
                        perror("Error: s flag issue");
                        return flags;
                    }
                    break;
                case 'S': 
                    S_flag = checker(optarg);
                    if(0 == S_flag){ //check return.
                        flags[0] = -1;
                        perror("Error: S flag issue");
                        return flags;
                    }
                    break;
                case 'v': 
                    v_flag = checker(optarg);
                    if(0 == v_flag){ //check return.
                        flags[0] = -1;
                        perror("Error: v flag issue");
                        return flags;
                    }
                    break;
                case 'U':  
                    U_flag = checker(optarg);
                    if(0 == U_flag){//check return.
                        flags[0] = -1;
                        perror("Error: U flag issue");
                        return flags;
                    }
                    break;
                case 'c':
                    c_flag = checker(optarg);
                    if(0 == c_flag){ //check return.
                        flags[0] = -1;
                        perror("Error: c flag issue");
                        return flags;
                    }
                    break;
                default :
                    break;
        }
    }

    if((argc-1)-optind >= 0){
       perror("Error: cmdline Non-Arguments detected!\n");
       flags[0] = -1;
       return flags;
    }
    optind = 1; //resets getopt.

    if(-1 == p_flag){ //no -p or no PID. Now get UID.
        flags[0] = -3;
        return flags;
    }

    flags[0] = pid; //sets flags
    flags[1] = s_flag;
    flags[2] = U_flag;
    flags[3] = S_flag;
    flags[4] = v_flag;
    flags[5] = c_flag;
    flags[6] = p_flag;
     
    return flags;
}

