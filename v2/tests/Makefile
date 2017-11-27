CC          := gcc

WARNINGS ?= -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wuninitialized -Wconversion -Wstrict-prototypes -Werror
			
CFLAGS := -std=c11 -Ofast $(WARNINGS) 

DEBUG := -g3
# CFLAG += -g3

LIBS := -lm

HDRSARR = ./test.h ../src/utils/arrays.h
SRCSARR =  ../src/utils/arrays.c ./test_arrays.c
OBJSARR = $(SRCSARR:.c=.o)

HDRSCSV = ./test.h ../src/utils/csv.h
SRCSCSV =  ../src/utils/csv.c ./test_csv.c
OBJSCSV = $(SRCSCSV:.c=.o)

all:
	make test_array
	make test_csv
	make clean

test:
	./test_array
	./test_csv
	
test_array: $(OBJSARR) $(HDRSARR) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSARR) $(LIBS)
	
test_csv: $(OBJSCSV) $(HDRSCSV) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSCSV) $(LIBS)

.PHONY: clean
clean:
	rm -f core $(EXE) *.o
	rm -rf ../src/*.o
	rm -rf ../src/utils/*.o
