# Makefile
#
# In this program there is 1 main program (main.c) and 2 helper
#programs. (queue_functions.c and action.c).
# Here is a list of the .c and .h files:
# main.c
# queue_functions.c 
# queue_functions.h
# queue_struct.h
# action.c
# action.h
# 
# Here is the dependency tree:
#
#
#
#
#          		 	   -- queue_functions.h -- queue_functions.c
#         			/
# prodcom -- -- (main.c) -- queue_struct.h
#         			\
#         		 		-- action.h-- action.c
#                       										
#
# Remember that if a module depends on another module,
# the .h file for its dependency must be included in
# the Makefile as a dependency (e.g. module2 depends
# on module3 so module3.h is listed as a dependency
# for the target module2.o)
#

CC      = gcc
CFLAGS = -Wall -pedantic -g
OBJECTS = main.o action.o queue_functions.o

proj2: $(OBJECTS)
        $(CC) $(CFLAGS) -pthread $(OBJECTS) -o proj2

main.o: main.c queue_functions.c action.c queue_functions.h action.h
        $(CC) $(CFLAGS) -c main.c

action.o: action.c action.h queue_functions.c queue_functions.h
        $(CC) $(CFLAGS) -c action.c

queue_functions.o: queue_functions.c queue_functions.h queue_struct.h
        $(CC) $(CFLAGS) -c queue_functions.c

clean:
        rm -f $(OBJECTS) proj2