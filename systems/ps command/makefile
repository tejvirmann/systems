# Makefile
#
# In this program there is 1 main program (main.c) and 2 helper
#programs. (read_proc.c and parse_cl.c)
# Here is a list of the .c and .h files:
# main.c
# parse_cl.c 
# parse_cl.h
# read_proc.c 
# parse_stat.h
# parse_statm.h
# parse_c.h
# get_proc.c
# 
# Here is the dependency tree:
#
#
#
#
#          		 	   -- parse_cl.c -- parse_h.h
#         			/
# 537ps -- -- (main.c) -- get_proc.c
#         			\
#         		 		-- read_proc.c -- parse_stat.h
#                       				\
#                       		 		 -- parse_statm.h
#										 \
#										   -- parse_c.h
#
#
# Remember that if a module depends on another module,
# the .h file for its dependency must be included in
# the Makefile as a dependency (e.g. module2 depends
# on module3 so module3.h is listed as a dependency
# for the target module2.o)
#

CC      = gcc
CFLAGS = -Wall -pedantic
OBJECTS = main.o parse_cl.o read_proc.o get_proc.o

537ps: $(OBJECTS)
 $(CC) $(CFLAGS) -o 537ps $(OBJECTS)

main.o: main.c parse_cl.c read_proc.c parse_cl.h parse_stat.h parse_statm.h parse_c.h get_proc.c
 $(CC) $(CFLAGS) -c main.c

parce_cl.o: parse_cl.c parse_cl.h
 $(CC) $(CFLAGS) -c parse_cl.c

read_proc.o: read_proc.c parse_stat.h parse_statm.h parse_c.h
 $(CC) $(CFLAGS) -c read_proc.c

get_proc.o: get_proc.c
 $(CC) $(CFLAGS) -c get_proc.c

clean:
 rm -f $(OBJECTS) 537ps