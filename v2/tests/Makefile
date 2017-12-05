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

HDRSSLL = ./test.h ../src/utils/sll.h
SRCSSLL =  ../src/utils/sll.c ./test_sll.c
OBJSSLL = $(SRCSSLL:.c=.o)

all:
	make test_array
	make test_csv
	make test_sll
	make clean

test:
	./test_array
	./test_csv
	./test_sll
	
test_array: $(OBJSARR) $(HDRSARR) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSARR) $(LIBS)
	
test_csv: $(OBJSCSV) $(HDRSCSV) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSCSV) $(LIBS)
	
test_sll: $(OBJSSLL) $(HDRSSLL) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSSLL) $(LIBS)

.PHONY: clean
clean:
	rm -f core $(EXE) *.o
	rm -rf ../src/*.o ../src/utils/*.o ./test_array ./test_csv ./test_csv ./test_sll
