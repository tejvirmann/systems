// /////////////////////////////////////////////////////////////////////////////

// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.

// //////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row
// This File:        n_in_a_row
// Other Files:      na
// Semester:         CS 354 Spring 2020

// Author:           Tejvir Mann
// Email:            tsmann@wisc.edu
// CS Login:         tejvir 

// ///////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.

// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.

// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
// //////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *COMMA = ",";  
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}       
 
   
  
/* TODO:
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {
    
    if (size/2 == 0){
        return 1;
    }
    
    //runs through horizontally, vertically, then diagonally 
    //keeps track of the number of X O s and if there are any matches vmatches, hmatches, or dmatches. 
    //if there are 2 of either, then invalid, but if 2 of them have one then its fine
    //if all goes well return 0 for valid, 1 for invalid. 

    //4 for loops. vertical. horizontal. diag. and to count the x and os. 
    
    //(1 and 3)this checks Xs and Os and checks hmatches
    int x; 
    int o;
    int k;
    int l;
    int hmatchX;
    int hmatchO;
    for (int i = 0; i < size; i++){
    	for(int j = 0; j < size; j++){
    		if(*(*(m+i)+j) == 1){
    			x += 1;
    			k += 1;
    		}

    		if(*(*(m+i)+j) == 2){
    			o += 1;
    			l += 1;
    		}

    		if(k == size){
    			hmatchX += 1;
    		}

    		if(l == size){
    			hmatchO += 1;
    		}

    	}
    	k = 0; //resets k and l for next array
    	l = 0; 
    }
    
    if(x+1 != o || x != o){
    	return 1; 
    }

    //(3) this loop checks vmatches
    i = 0; //resets i and j 
    j = 0;
    q = 0;
    p = 0;
    int vmatchX; 
    int vmatchO;
    for (int j = 0; j < size; j++){
    	for(int i = 0; i < size; i++){
    		if(*(*(m+i)+j) == 1){
    			q += 1;
    		}

    		if(*(*(m+i)+j) == 2){
    			p += 1;
    		}
    	}

    	if(q == size){
    		vmatchX += 1;
    	}

    	if(p == size){
    		vmatchO += 1;
    	}

    	q = 0;
    	p = 0;
    }

    i = 0; //resets i and j 
    j = 0;
    dmatchX2 = 0;
    dmatchO2 = 0;
    //(4.a) checks if there is a diagonal match from top left to bottom right 
    for (int i = size; i > 0; i--){
    	if(*(*(m+i)+i) == 1){
    		y += 1;
    	}

    	if(*(*(m+i)+i) == 2){
    		z += 1;
    	} 
    }

    if(y == size){
    	dmatchX2 += 1; 
    }

    if(z == size){
    	dmatchO2 += 1; 
    }


    //(4.b) checks diagonal from top right to bottom left 
    i = 0;
    int y = 0;
    int z = 0;
    int dmatchX = 0;
    int dmatchO = 0;
    for (int i = 0; i < size; i++){
    	if(*(*(m+i)+i) == 1){
    		y += 1;
    	}

    	if(*(*(m+i)+i) == 2){
    		z += 1;
    	} 	
    }

    if(y == size){
    	dmatchX += 1; 
    }

    if(z == size){
    	dmatchO += 1; 
    }

//at this point, vmatchX, vmatchO, hmatchX, hmatchO, dmatchX, dmatchO, dmatchX2, dmatchO2
//returns 1 if not valid 

    if(hmatchX >=1 && hmatchO >=1){
    	return 1; 
    }

    if(vmatchX >=1 && vmatchO >=1){
    	return 1; 
    }

    if(dmatchX >=1 && dmatchO >=1){
    	return 1; 
    }

    if(hmatchX > 1 || hmatchO > 1 || vmatchX > 1 || vmatchO > 1 || dmatchX > 1 || dmatchO> 1 || dmatchX2> 1 || dmatchO2 > 1){
    	return 1;
    }

    if(hmatchX + vmatchX + dmatchX + dmatchX2 > 2){
    	return 1;
    }

    if(hmatchO + vmatchO + dmatchO + dmatchO2 > 2){
    	return 1;
    }

    return 0;   
}    
  
 
   
/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    //TODO: Check if number of command-line arguments is correct.
    if(argc != 1){
    	exit(1)
    }

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;

    //TODO: Call get_dimensions to retrieve the board size.
    get_dimensions(*fp, *size);

    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    int **m; 
    m = malloc(sizeof(int*)*size);
    for(int i = 0; i < size; i++){
        m[i] = malloc(sizeof(int)*size);
    }

    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, COMMA);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            /* ADD ARRAY ACCESS CODE HERE */ 
               *(*(m+i)+j) = atoi(token);  //this should mean m[i][j]
            token = strtok(NULL, COMMA);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    int result; 
    result = n_in_a_row(**m, size); // **m reference to board, size is just an int.

    //TODO: Free all dynamically allocated memory.
    for(int i = 0; i < size; i ++){
        free(m[i]);
    }
    free(m);
    m = NULL;

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       
