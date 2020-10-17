/*
Project: 537ps
File: get_proc.c
All Files: main.c, parse_cl.c, parse_cl.h, read_proc.c, parse_stat.h, 
parse_statm.h, parse_c.h, get_proc.c
Authors: Tejvir Mann and Ryann Mahajan
Class: CS 537 OS
About this file:
This file is called by main and this method runs if there was no pid
provided in the command line. This method gets all the pids of the 
associated uid. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

/*
This method is called by get_proc. It is responsible for scanning and 
getting the first integer of the data. 
param: str: this is used. 
output: this outputs an integer a which is the pid. 
*/
int get_first_integer(char* str){
    char digits[] = "0123456789";
    char *ptr = strpbrk(str, digits);

    if (ptr != NULL) /* Expected character is found */
        {
            int a;
            sscanf(ptr, "%d ", &a);
            return a;

        }
    return 0;
}

/*
compare to uid. if equal, take pid. Add to the array. ATOI!!!
run through each ./proc get the uid in status.
compare these uids.
if same add to proc array. 
return proc array.
returns a pid array
*/
int * get_proc() {
    
    //declare the int array.
    const int SIZE = 1000;
    int i;
    int *pids; 
    int cur_uid;
    int count;
    int uid;
    int proc_file;
    char *filename;

    //alloc
    pids = malloc(SIZE * sizeof(int));
    filename = (char*) malloc(SIZE*sizeof(char));
    if(NULL == (pids || filename)){
        perror("Error: Memory Allocation: get_proc");
        pids[0] = -1;
        return pids;
    }

    //get uid
    uid = getuid();
    i = 0; //set variables

    //get to proc/only integer pid/status/UID.
    DIR *folder;
    struct dirent *entry;

    folder = opendir("/proc/"); //open file
    if (folder == NULL) {
        perror("couldn't open '/proc/'");
        pids[0] = -1;
        return pids;
    }

    while( (entry=readdir(folder)) ){ //run through each pid file 
        count = 0;
        proc_file = atoi(entry -> d_name);
        if(0 == proc_file){
            //continue;
        }
        else{
            sprintf(filename, "/proc/%d/status", proc_file); /* should check that argc > 1 */
            FILE* file = fopen(filename, "r"); /* should check the result */
            char line[256];

            while (fgets(line, sizeof(line), file)) {  //get to the 9th line in status.
                if(8 == count){
                    //now you are at line with UID.
                    cur_uid = get_first_integer(line); //I believe this gets the first int (real UID) seen when scanning.
                    if(cur_uid == uid){ //if uids match
                        pids[i] = proc_file; //save pid id
                        i++; 
                    }
                }
                else{}
        
                count++;
            }

            fclose(file);
        }
    }

    return pids;
}