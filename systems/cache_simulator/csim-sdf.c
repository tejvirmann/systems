////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * csim.c:  
 * A cache simulator that can replay traces (from Valgrind) and output
 * statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most 1 cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************/
/* DO NOT MODIFY THESE VARIABLES **********************************************/

//Globals set by command line args.
int b = 0; //number of block (b) bits
int s = 0; //number of set (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set

//Global command tracker. To be found in replay
//and used in access_data
char command = ' '; //will be set to L, or S
char command2 = ' ';  //will be set to M when needed
int counter = 1; //keeps track of the count
/******************************************************************************/
  
  
//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t;

//Type cache_line_t: Use when dealing with cache lines.
//TODO - COMPLETE THIS TYPE
typedef struct cache_line {                    
    char valid;
    mem_addr_t tag;
    //Add a data member as needed by your implementation for LRU tracking.
    //a variable to keep track of the last time the line was accessed
    int count;
    //use counter++
} cache_line_t;

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;

//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t; //this is equal to a pointer for cache. 

// Create the cache (i.e., pointer var) we're simulating.
cache_t cache;  

/* TODO - COMPLETE THIS FUNCTION
 * init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() {      
    B = pow(2,b); 
    S = pow(2,s); 

    cache = (cache_t)malloc(sizeof(char*) *S); //allocates an array for sets 
    for(int i = 0; i < S; i++){
        cache[i] = (cache_set_t)malloc(sizeof(char*) *E); //allocates an array for the memory blocks
        for(int j = 0; j < E; j++){  //for each element in set
            cache[i][j].valid = '0';
            cache[i][j].tag = 1232;
            cache[i][j].count = 0;
        }
    }  
}
  

/* TODO - COMPLETE THIS FUNCTION 
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {    

    //for loop through everything and free the hebrews
    for(int i = 0; i < S; i++){
        free(cache[i]);
        cache[i] = NULL;
    }  

    free(cache);
    cache = NULL;
}
   
   
/* TODO - COMPLETE THIS FUNCTION 
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cnt
 */                    
void access_data(mem_addr_t addr){

    //get the t-bit address and the s-bit address
    unsigned long long int addr_s = addr; //the t and s starting addys
    unsigned long long int addr_t = addr; 

    //printaddress:
    printf("%llu\n", addr);
    
    int t = 64 - s - b; 

    addr_t = addr_t >> (s+b); //this is the t-bits >>> - padding with 0's
                                                    //>> paddings with most sig bit

    addr_s = addr_s << t;
    addr_s = addr_s >> (b+t); //this is the b -bits

    //print address s and t
    //printf("%llu\n", addr_t);
    //printf("%llu\n", addr_s);

    int total_invalid = 0;
    int smallest_count = 100000; //used to determine smallest count in evic curcumstances 
    //int hit = 0;
     
    //printf("%llu\n",(unsigned long long int)&cache);
    //printf("%llu\n",(unsigned long long int)&cache[0]);
    printf("%llu\n",((unsigned long long int)&cache[0] -8));
    unsigned long long int addr_tofind = ((unsigned long long int)&cache[0]-8) +((int)addr_s)*8; 

    //print address to find
    printf("%llu\n", addr_tofind);

    printf("%c\n", 'G');
   // printf("%p\n", &cache[0]-8);
   // printf("%p\n", &cache[0]);
  //  printf("%p\n", &cache[1]);
    
    if ('L' == command){
        for(int i = 0; i <S; i++){
            //print &cache[i]
            //printf("%llu\n", (unsigned long long int)&cache[i]);

            if(addr_tofind == (unsigned long long int)&cache[i]){ //Finds the right S.
                printf("%c\n", 'L');
                for(int j = 0; j < E; j++){
                    if(cache[i][j].tag == addr_t){ //finds t 
                        hit_cnt += 1; //if count then hit
                        //hit = 1;
                        cache[i][j].count = counter; 
                        printf("%s\n", "hit");
                        return;
                    }

                    if(cache[i][j].valid != '0'){
                        total_invalid += 1; //counts invalids
                    }
                }


                if(total_invalid == E){ 
                //then none valid, and eviction then load.
                //a miss and eviction but add it
                //find the line with the smallest count and insert
                    for(int j = 0; j < E; j++){ ///DONT GET THIS
                        //cache[i][j].count == smallest_count;
                        if(cache[i][j].count < smallest_count){
                            smallest_count = cache[i][j].count ;
                        }
                    }

                    for(int j = 0; j < E; j++){
                        if(cache[i][j].count == smallest_count){
                            cache[i][j].tag = addr_t;
                            cache[i][j].count = counter;
                            miss_cnt += 1;
                            evict_cnt += 1;
                            printf("%s\n", "miss and evic");
                            return;
                        }
                    }
                }

                else{ 
                    //just a normal miss
                    //run through find a valid and insert
                    for(int j = 0; j < E; j++){
                        if(cache[i][j].valid == '0'){
                            cache[i][j].valid = '1';
                            cache[i][j].tag = addr_t;
                            cache[i][j].count = counter;
                            miss_cnt += 1;
                            printf("%s\n", "miss");
                            return;
                        }
                    }
                } 
            }
        }
    }

    if('S' == command){
        for(int i = 0; i < S; i++){
            printf("%llu\n", (unsigned long long int)&cache[i]);
            if(addr_tofind == (unsigned long long int)&cache[i]){
                printf("%c\n", 'S');
                for(int j = 0; j < E; j++){
                    if(command2 == 'M'){
                        if(cache[i][j].tag == addr_t){
                            hit_cnt += 1; //if count then hit
                            //hit = 1;
                            cache[i][j].count = counter; 
                            cache[i][j].tag = addr_t; 
                            printf("%s\n", "hit M");
                            return;
                        }
                        continue; //dont care about the other if, since M.
                    }

                    if(cache[i][j].valid == '0'){ //finds open space, for S.
                        hit_cnt += 1; //if count then hit
                        //hit = 1;
                        cache[i][j].count = counter; 
                        cache[i][j].tag = addr_t; 
                        printf("%s\n", "hit S");
                        return;
                    }
                }

                for(int j = 0; j < E; j++){ //finds the smallest count  //DONT GET THIS
                    //cache[i][j].count == smallest_count;
                    if(cache[i][j].count < smallest_count){
                        smallest_count = cache[i][j].count;
                    }
                }

                for(int j = 0; j < E; j++){
                    if(cache[i][j].count == smallest_count){ //if smallest count
                        cache[i][j].tag = addr_t;
                        cache[i][j].count = counter;
                        miss_cnt += 1;
                        evict_cnt += 1;
                        printf("%s\n", "miss an evic S");
                        return;
                    }
                }
            } 
        }
    }
}
  
/* TODO - FILL IN THE MISSING CODE
 * replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */                    
void replay_trace(char* trace_fn) {           
    char buf[1000];  
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r"); 

    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);   
    }

    while (fgets(buf, 1000, trace_fp) != NULL) {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);
      
            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

            // GIVEN: 1. addr has the address to be accessed
            //        2. buf[1] has type of access(S/L/M)
            // call access_data function here depending on type of access

            if (buf[1] == 'L'){
                command = 'L';
               // printf("%c\n", buf[1]);
                access_data(addr); //assuming the addr is stored
            }

            if (buf[1] == 'S'){
                command = 'S';
               // printf("%c\n", buf[1]);
                access_data(addr);
            }

            if (buf[1] == 'M'){
                command2 = 'M';
                printf("%c\n", 'M');
                command = 'L';
                access_data(addr);

                command = 'S';
                access_data(addr);
                command2 = ' ';
            }
            
            if (verbosity)
                printf("\n");
        }

        counter += 1;
    }

    fclose(trace_fp);
}  
  
  
/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of s bits for set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of b bits for block offsets.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}  
  
  
/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}  
  
  
/*
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL;
    char c;
    
    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'h':
                print_usage(argv);
                exit(0);
            case 's':
                s = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    //Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage(argv);
        exit(1);
    }

    //Initialize cache.
    init_cache();

    //Replay the memory access trace.
    replay_trace(trace_file);

    //Free memory allocated for cache.
    free_cache();

    //Print the statistics to a file.
    //DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt);
    return 0;   
}  