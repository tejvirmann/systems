/*
 * Project: 537ps
 * File: main.c
 * All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
 parse_statm.h, parse_c.h, get_proc.c
 * Authors: Tejvir Mann and Ryann Mahajan
 * Class: CS 537 OS
 * About this file:
 * This is the main file responsible for using all the methods within other files to 
 * parse the command line, and get the process information. 
 */

//libs
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

//declare funcs
int read_proc(int *flags);
int * parse_cl(int argc, char **argv, int pid_p);
int * get_proc();

/*
This method is where we call other methods and is the main method
where each of the other programs and methods are called from. 
params: argc - the # of argvs passed in. argv - the string from the cmdline.
output: prints info returns -1.
*/
int main(int argc, char **argv) {

	//dec vars
	int i = 0;
	int pid_p = -1; 
	int *flags;
	int *pids;

	//get flags
	flags = parse_cl(argc, argv, pid_p);  
	if(NULL== flags){ //check
	   perror("Error: Could not allocate space\n");
	   return -1;
	}

	if (-1 == flags[0]){ //file error catch
		return -1;
	}
	else if(-3 == flags[0]){ //where no pid specified.
		pids = get_proc(); //returns an array of PIDS.
		if (-1 == pids[0]){ //if error getting pids
			perror("Error: UID has no procs!\n");
			return -1;
		}
		i = 0;
		while(0 != pids[i]){ //loops through pids, and prints info.
		    flags = parse_cl(argc, argv, pids[i]); //gets flags.
			if (-1 == flags[0]){ //then error.
				return -1;
			}
			if(-1== read_proc(flags)){ //gets and prints info.
				return -1;
			}
			i++;
		}
	}
	else{  	//if pid given, and now we need to get and print proc info.
		read_proc(flags);
	}

	return -1; //at end.
}