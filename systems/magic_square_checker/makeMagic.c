////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        makeMagic.c
// This File:        makeMagic.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Spring 2020
//
// Author:           Tejvir Mann
// Email:            tsmann@wisc.edu
// CS Login:         tejvir 
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;     // dimension of the square
    int **msquare; // pointer to heap allocated magic square
} MSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {

    int a;

    printf("Enter magic square's size (odd integer >=3)\n");
    scanf("%d", &a);

    if (a < 3){
        printf("Size must be >= 3.\n");
        exit(1);
    }

    if(a%2 == 0){
        printf("Size must be odd.\n");
        exit(1);
    }

    return a;   
} 
   
/* TODO:
 * Makes a magic square of size n using the Siamese algorithm
 * and returns a pointer to the completed MSquare struct.
 *
 * n the number of rosw and columns
 */
MSquare *generateMSquare(int n) {

    MSquare *values;
    values = malloc(sizeof(MSquare));
    if(values == NULL){
        exit(1);
    }
    
    int **table = malloc(sizeof(int*) *n);
    if (table == NULL){
        exit(1);
    }

    //making space
    for(int i = 0; i < n; i++){
        *(table+i) = (int *)malloc(n * sizeof(int));
        if(*(table+i) == NULL){
            printf("Memory Allocation Error\n");
            exit(1);
        }
    }

    //make all the values 0 
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            *(*(table + i) + j) = 0;
        }
    }

  int nsqr = n * n;
  int i=0, j=n/2;     // start position
 
  for (int k=1; k<=nsqr; ++k){// k = 1, k<= nsqr
    *(*(table+i)+j) = k;

    i--;
    j++;
     
    if (k%n == 0) 
    { 
      i += 2; 
      --j; 
    }
    else 
    {
      if (j==n) 
        j -= n;
      else if (i<0) 
        i += n;
    }
  }
   
    values -> msquare = table;
    values -> size = n;
    

    return values;    
    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * msquare the magic square to output
 * filename the name of the output file
 */
void outputMSquare(MSquare *msquare, char *filename) {
    FILE *fp;
    fp = fopen(filename, "w");
    if (fp == NULL) { //check if not null
        printf("Can't open file for reading.\n");
        exit(1);
    }
    
    //make a for loop fprint 
    int **table = msquare -> msquare;
    fprintf(fp, "%i\n", msquare -> size); 

    for(int i = 0; i < msquare-> size; i++){
        for(int j =0; j< msquare-> size; j++){
            fprintf(fp, "%i", *(*(table + i) +j));
                if(j == (msquare -> size)-1){
                    fprintf(fp, "\n");
                }
                else{
                    fprintf(fp, ",");
                }
        }
    }
   
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 

}

/* TODO:
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {

    // TODO: Check input arguments to get output filename
    if(argc != 2){
        printf("Usage: ./makeMagic <output_filename>\n" );
        exit(1);
    }

    FILE *fp = fopen(*(argv + 1), "w");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }
    
    char *name; 
    name = *(argv +1);

    // TODO: Get magin square's size from user
    MSquare *values;
    values = malloc(sizeof(MSquare));
    if(values == NULL){
        exit(1);
    }
    values -> size = getSize();
     
    // TODO: Generate the magic square
    values = generateMSquare(values -> size);
    
   
    // TODO: Output the magic square
    outputMSquare(values, name);
    
    //free all the space
    int **clear = values -> msquare; 

    for(int i = 0; i < values -> size; i++){
        free(clear[i]); //frees the pointers 
    }
    
    free(values);
    free(clear);
    values = NULL;
    clear = NULL; 

        //Closes the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 

    return 0;
} 