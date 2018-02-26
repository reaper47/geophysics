CC          := gcc

WARNINGS ?= -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wuninitialized -Wconversion -Wstrict-prototypes -Werror
			
CFLAGS := -std=c11 -O3 $(WARNINGS) 

DEBUG := -g3 -fsanitize=address
#CFLAG += -ggdb

LIBS := -lm -ljansson

HDRSARR = ./test.h ../include/arrays.h
SRCSARR = ../src/utils/arrays.c ./test_arrays.c
OBJSARR = $(SRCSARR:.c=.o)

HDRSCSV = ./test.h ../include/csv.h ../include/strings.h
SRCSCSV = ../src/utils/csv.c ../src/utils/sll.c ./test_csv.c ../src/utils/strings.c
OBJSCSV = $(SRCSCSV:.c=.o)

HDRSSLL = ./test.h ../include/sll.h ../include/strings.h
SRCSSLL = ../src/utils/sll.c ./test_sll.c ../src/utils/strings.c
OBJSSLL = $(SRCSSLL:.c=.o)

HDRSGRAV = ./test.h ../include/sll.h ../include/gravimetry.h ../include/csv.h ../include/strings.h ../include/calcs.h ../include/topo.h ../include/arrays.h
SRCSGRAV = ../src/utils/sll.c ../src/gravimetry.c ../src/utils/csv.c ./test_gravimetry.c ../src/utils/strings.c ../src/utils/calcs.c ../src/topo.c ../src/utils/arrays.c
OBJSGRAV = $(SRCSGRAV:.c=.o)

HDRSSTR = ./test.h ../include/strings.h
SRCSSTR = ../src/utils/strings.c ./test_strings.c
OBJSSTR = $(SRCSSTR:.c=.o)

HDRSCALCS = ./test.h ../include/calcs.h
SRCSCALCS = ../src/utils/calcs.c ./test_calcs.c
OBJSCALCS = $(SRCSCALCS:.c=.o)
	
HDRSTOPO = ./test.h ../include/sll.h ../include/topo.h ../include/csv.h ../include/strings.h ../include/arrays.h ../include/calcs.h
SRCSTOPO = ../src/topo.c ../src/utils/sll.c ../src/utils/csv.c ./test_topo.c ../src/utils/strings.c ../src/utils/arrays.c ../src/utils/calcs.c
OBJSTOPO = $(SRCSTOPO:.c=.o)

all:
	make test_arrays
	make test_csv
	make test_sll
	make test_grav
	make test_strings
	make test_calcs
	make test_grav
	make test_topo
	make clean

test:
	./test_arrays
	./test_csv
	./test_sll
	./test_strings
	./test_grav
	./test_topo
	
test_arrays: $(OBJSARR) $(HDRSARR) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSARR) $(LIBS) $(DEBUG)
	
test_csv: $(OBJSCSV) $(HDRSCSV) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSCSV) $(LIBS) $(DEBUG)
	
test_sll: $(OBJSSLL) $(HDRSSLL) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSSLL) $(LIBS) $(DEBUG)
	
test_grav: $(OBJSGRAV) $(HDRSGRAV) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSGRAV) $(LIBS) $(DEBUG)
	
test_strings: $(OBJSSTR) $(HDRSSTR) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSSTR) $(LIBS) $(DEBUG)

test_calcs: $(OBJSCALCS) $(HDRSCALCS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSCALCS) $(LIBS) $(DEBUG)

test_topo: $(OBJSTOPO) $(HDRSTOPO) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJSTOPO) $(LIBS) $(DEBUG)
	
.PHONY: clean
clean:
	rm -f core $(EXE) *.o
	rm -rf ../src/*.o ../src/utils/*.o
	
.PHONY: purge
purge:
	rm -f core $(EXE) *.o
	rm -rf ./test_data/processed/*.csv ../include/*.o ../src/*.o ../src/utils/*.o ./test_arrays ./test_csv ./test_csv ./test_sll ./test_grav ./test_strings ./test_calcs ./test_topo
	
