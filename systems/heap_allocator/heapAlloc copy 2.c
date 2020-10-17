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
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */
void* lastAlloc = NULL;

 
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
    int padding = 0;
    int times = 0;

    //do I need to set the size_status of heap start? 

    if(size <= 0){
        return NULL;
    }

    /*if(size > allocsize){  //not sure how to get heapsize, also where do we call intialheap and setup ptrs?
        return NULL;
    }*/

    if(lastAlloc == NULL){
        lastAlloc = heapStart;
    }
    int totalSize = size + 4; //4 for header.

    if(totalSize % 8 != 0){
        padding = 8 - (totalSize % 8);
        totalSize = totalSize + padding;
    }

    //find a free block, so run through somehow
    //blockHeader* ptr = NULL;
    //ptr = heapStart;

    //if(lastAlloc != NULL){  //for next fit, depends on most recent allocation. 
    //    ptr = lastAlloc; 
    //}
    blockHeader* ptr = lastAlloc;
    int flag = 0;

    while(ptr->size_status != 1){  //the end of availible memory :also: *ptr.size_status()!= 1 
        
        if(ptr -> size_status % 8 == 0){
            if(ptr -> size_status > totalSize){  //see if right size 
                //check if the extra space can be made into a free block. 
                if((ptr -> size_status) - totalSize >= 8){  //maybe switch these around
                    //if there are 17 bytes of space, then the footer
                    //times = int((totalSize - (ptr -> size_status)) / 8);
                    times = (totalSize/8) + 1; //int(totalSize/8) + 1;
                    
                    //special case: if totalSize is a multiple of 8. 
                    if(totalSize % 8 == 0){
                        times = (totalSize/8);  //used to have int cast
                    }

                    ((blockHeader*)((void*)ptr + (8*times))) -> size_status = (ptr -> size_status) - totalSize;
                    //above adds a footer at that specific address
                }

                if(ptr == heapStart){
                    totalSize = totalSize + 3; //this is for first allocation at the beginning.
                }

                //update footer into a header
                ((blockHeader*)((void*)ptr)) -> size_status = totalSize + 0; //plus 0 since previous block free
                lastAlloc = ptr;

                return ptr;
            }
        }

        if(ptr -> size_status % 8 == 2){
            if(ptr -> size_status > totalSize + 2){

                if((ptr -> size_status - 2) - totalSize >= 8){ //checks if extra space enough.
                    //times = int((totalSize - (ptr -> size_status - 2)) / 8);
                    times = (totalSize/8) + 1; //used to have int cast 
                    
                    //special case: if totalSize is a multiple of 8. 
                    if(totalSize % 8 == 0){
                        times = (totalSize/8); //used to have int cast 
                    }

                    ((blockHeader*)((void*)ptr + (8*times))) -> size_status = (ptr -> size_status) - totalSize; //should contain the +2, since previous is allocated. 
                    //above adds a footer at that specific address
                }

                //update footer. 
                ((blockHeader*)((void*)ptr)) -> size_status = totalSize + 2; //+2 since previous block not free 
                lastAlloc = ptr;

                return ptr;
            }
        }

       //((blockHeader*)((void*)ptr)) = (ptr + ((ptr -> size_status) - ((ptr -> size_status) % 8)));
       ptr = (blockHeader*)((void*) ptr + ((ptr -> size_status) - ((ptr -> size_status) % 8)));

        //ptr = ptr + 8;  //instead of 8, just add the previous pointer size, instead of 8. 
    }

    return NULL; //if no address could be found 
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
    //TODO: Your code goes in here.
    //((blockHeader*)((void*)ptr + (8*times)))

    if(ptr == NULL){
        return -1;
    }

    if((((unsigned long) ptr) % 8) != 0){  //its all casting errors now
        return -1; 
    }

    if (((blockHeader*)ptr) < heapStart || ((blockHeader*)ptr) > (heapStart + allocsize)){
        return -1;
    }

    if((((blockHeader*)ptr) -> size_status) % 2 == 0){
        return -1;
    }

    //just like before, run through the entire memory. 
    //keep track of the memory before and after. 
    //once you reach the pointer, change it to free. 
    //check if the before and after are free, and add
    //accordingly. 
    //update the footers and such

    blockHeader* curr = heapStart;
    curr = heapStart;
    blockHeader* ptr_left = NULL; 
    blockHeader* ptr_right = curr + ((curr -> size_status) - ((curr -> size_status) % 8));
    
    while(curr -> size_status != 1){

        if(curr == ptr){

            /*if(ptr_left == NULL){
                ptr_left = 1;
            }*/

            if(ptr_right < heapStart + allocsize){
                if(ptr_right -> size_status % 2 == 0){ //checks if right is free
                    if(ptr_left -> size_status % 2 == 0){ //all three free
                        int a = ((curr -> size_status) - ((curr -> size_status) % 8));  //gets the number - the fluf to get actual size. 
                        int b = ((ptr_right -> size_status) - ((ptr_right -> size_status) % 8));
                        ptr_left -> size_status = ptr_left -> size_status + a + b - 1;  //-1 to make curr block free
                        return 0;
                    }

                    else{
                        //add the right to curr
                        curr -> size_status = curr -> size_status + ((ptr_right -> size_status) - ((ptr_right -> size_status) % 8)) -1;
                        return 0;
                    }
                }

                else{
                    if(ptr_left -> size_status % 2 == 0){ //add curr to left
                        ptr_left -> size_status = ptr_left -> size_status + ((curr -> size_status) - ((curr -> size_status) % 8)) -1;
                        return 0;
                    }
                        
                    else{
                        curr -> size_status = curr -> size_status - 1; 
                        return 0;
                    } 
                }
            }

            else{
                if(ptr_left -> size_status % 2 == 0){ //add curr to left
                    ptr_left -> size_status = ptr_left -> size_status + ((curr -> size_status) - ((curr -> size_status) % 8)) -1;
                    return 0;
                }
                    
                else{
                    curr -> size_status = curr -> size_status - 1; 
                    return 0;
                    } 
            }
        }
        
        ptr_left = curr; //the current one
        curr = curr + ((curr -> size_status) - ((curr -> size_status) % 8));
        ptr_right =  curr + ((curr -> size_status) - ((curr -> size_status) % 8)); //the next next one
        
    }


    return -1;
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