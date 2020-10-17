///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "heapAlloc.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *end = NULL;  //for the last block that is allocated at the end
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;


/**
 *  This method takes a block of memoray and splits the 
 * block with some being allocated and some not being allocated.
 *
 * tobe_splitted   : block to split
 * allocated_size : size for memory alloc
 */
void split_block(blockHeader* tobe_splitted, int allocated_size) {

    // this splits the block size between allocated and free
    int sblock_size = (tobe_splitted->size_status - 
                                  (tobe_splitted->size_status % 8)) - allocated_size;
    

    // Sets the header for alloc block
    blockHeader* the_header = (blockHeader*) ((void*) tobe_splitted + allocated_size);
    the_header->size_status = sblock_size + 2; // Set pbit of header to be split

    // Set the footer of new free area
    blockHeader* the_footer = (blockHeader*) 
            ((void*) the_header + sblock_size - sizeof(blockHeader));

    the_footer->size_status = sblock_size;
}


 /**
 * Retreives prev block 
 *
 * returns the pointer to the last block, or NULL.
 */
blockHeader* prev_block(blockHeader* block) {
    if (block == NULL) {
        return NULL;
    }

    // gets the footer to get the previous block
    blockHeader* end_foot = (blockHeader*)((void*) block - sizeof(blockHeader));

    return (blockHeader*) ((void*) block - end_foot->size_status);
}

/**
 * Gets the next block, if there is no next block,
 * then returns: NULL.
 */
blockHeader* next_block(blockHeader* the_block) {            
    return (the_block == NULL) ? NULL : (blockHeader*) ((void*) the_block + 
                                    (the_block->size_status - (the_block->size_status % 8))); 
}

/*
This method makes sure coalescing happens whenever a block is freed 
in the free method.

returns a new blockheader of the coalesced block 
*/
blockHeader* combine(blockHeader* block_free) {
    int block_size = block_free->size_status - (block_free->size_status % 8); //gets block_size

    blockHeader* next_free = next_block(block_free); //check if next freed

    if (next_free->size_status != 1 && !(next_free->size_status & 1)) { //checks if freed wasn't at end
         block_size += next_free->size_status - (next_free->size_status % 8); //of heap
    } 

    if (!(block_free->size_status & 2)) {  //checks if previous block freed
        blockHeader* the_prev_block = prev_block(block_free);
        block_size += the_prev_block->size_status - (the_prev_block->size_status % 8);
        if (end == block_free) { //if last block, then move pointer
            end = the_prev_block;
        }
         
        block_free = the_prev_block; //sets the address of block free right.
    }

    // Set size status to freed with prev allocated
    block_free->size_status = block_size + 2; 

    return block_free;
}

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* allocHeap(int size) {
    if (size <= 0) {  //returns null if negative 
        return NULL;
    }   

    int block_length = size + sizeof(blockHeader);

    while (block_length % 8 != 0) { //to get the correct length
        ++block_length;
    }
 
    if (block_length > allocsize) { //then too large 
        return NULL; 
    } 
    else if (end == NULL) { //then allocate at beginning 
        end = heapStart;
    }

   // return next(block_size);
    int allocated_size = block_length;

    blockHeader* curr_block = end; // stores the curr block
    block_length = 0; // Tracks + resets the curr block

    int bit = 2; // keeps status of previous block

    do {   
        block_length = curr_block->size_status - (curr_block->size_status % 8);
        if (curr_block->size_status & 1) { //if allocted bit is 1 
            bit = 2;
        
        } else { // else free
            if (block_length >= allocated_size) {
                if (block_length > allocated_size) {  //if curr block greater, split
                    split_block(curr_block, allocated_size);
                }   
                end = curr_block; // Set last allocated block.
                curr_block->size_status = allocated_size + bit + 1; // Set bit to one.
                return ((void*) curr_block + sizeof(blockHeader)); // Returns payload ptr
            }
            bit = 0; // If free size too small, set bit to 0.
        }
        curr_block = (curr_block == NULL) ? NULL //to next block if no free found.
                : (blockHeader*) ((void*) curr_block + (curr_block->size_status - (curr_block->size_status % 8))); 

        if (curr_block->size_status == 1) {
            curr_block = heapStart;
        }
    } while (curr_block != end); //  when get back to starting block.

    return NULL; // if reach the end, then no free blocks. 
} 


/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int freeHeap(void *ptr) {    
    if (ptr == NULL) { // returns -1 if null pointer
        return -1;
    }
    unsigned long int value = (unsigned long int) ptr; //checks value bounds
    if(value % 8){ //if value is not within the contraints
        return -1;
    }
    //bound 1
    unsigned long int start = (unsigned long int) heapStart + sizeof(blockHeader*);
    unsigned long int final = (unsigned long int) heapStart + (unsigned long int) allocsize 
                                                                - sizeof(blockHeader*);
                                                                //gets bound 2
    if(value < start){
        return -1;
    }
    if(value > final){
        return -1;
    }
    blockHeader* free_block = (blockHeader*) (ptr - sizeof(blockHeader)); 

    if (!(free_block->size_status & 1)) { // Return -1 if freed  
        return -1; 
    }
    
    free_block->size_status -= 1;
    free_block = combine(free_block);
    blockHeader* footer = (blockHeader*)((void*) free_block + //sets the footer 
                            (free_block->size_status - (free_block->size_status % 8))
                                - sizeof(blockHeader));

    footer->size_status = (free_block->size_status - 
                                            (free_block->size_status % 8));
                     //sets its size status
    blockHeader* next_curr = next_block(free_block); 

    if(next_curr->size_status & 2){
        if(next_curr->size_status != 1){
            next_curr->size_status -= 2;
        }
    }

    return 0;
} 

 
/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int initHeap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple initHeap calls
 
    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dumpMem() {     
 
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
} 