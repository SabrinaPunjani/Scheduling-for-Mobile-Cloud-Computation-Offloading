
################################################################################
#
# A simple makefile version for Simlib projects.
#
################################################################################

# Define the C compiler.
#
CC=gcc

# Define the delete command.
RM=rm -f 

# Include all compiler warnings and allow for debugging.
#
CFLAGS = -Wall -g 

# Our target executable.
#
EXECUTABLE=run

# All header files are in the current directory
#
INCLUDE_DIR=.

# Link to the standard math library, libm.
#
LIBS=m

################################################################################

# Get a list of all the C source files in this directory.
#
SOURCES=$(wildcard *.c)

# These are the corresponding object files.
#
OBJECTS=$(SOURCES:.c=.o)

# Get a list of all the C header files in this directory.
#
INCLUDES=$(wildcard *.h)

################################################################################

all: $(EXECUTABLE)

# Make all object files dependent on all header files. If a header file changes,
# it will trigger a rebuild of everything. This is ok since the number of files
# is very low. This is easier than finding all individual dependencies.
#
$(OBJECTS): $(INCLUDES)

# How to build the executable.
#
$(EXECUTABLE): $(OBJECTS) $(INCLUDES)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $(OBJECTS) -l$(LIBS)

# How to build object files from source files. This uses the old fashion suffix
# rule format.
#
.c.o:
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $<  -o $@

# Clean things up so we can rebuild everything.
#
clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)

help:
	@echo "make all (default)|clean|help"

.PHONY: clean

################################################################################


