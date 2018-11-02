############## COMPILING & FLAGS
CC = gcc
CFLAGS =  -Wall -Wextra -pedantic -ansi
LDLIBS = -lodbc

############## PATHS
SRCPATH = ./src/
OBJPATH = ./obj/
HDRPATH = ./inc/

############## BINARIES
EXE = appreq appuser appfollow apptweet

############## BINARIES

appreq: appreq.o odbc.o
	$(CC) $(CFLAGS) $(LDLIBS) -o appreq $(OBJPATH)appreq.o $(OBJPATH)odbc.o

appuser: appuser.o odbc.o
	$(CC) $(CFLAGS) $(LDLIBS) -o appuser $(OBJPATH)appuser.o $(OBJPATH)odbc.o

appfollows: appfollows.o odbc.o
	$(CC) $(CFLAGS) $(LDLIBS) -o appfollows $(OBJPATH)appfollows.o $(OBJPATH)odbc.o

apptweet: apptweet.o odbc.o
	$(CC) $(CFLAGS) $(LDLIBS) -o apptweet $(OBJPATH)apptweet.o $(OBJPATH)odbc.o

############## OBJECTS

appreq.o: $(SRCPATH)appreq.c $(HDRPATH)appreq.h $(HDRPATH)odbc.h
	$(CC) $(CFLAGS) $(LDLIBS) -c $(SRCPATH)appreq.c -o $(OBJPATH)appreq.o

appuser.o: $(SRCPATH)appuser.c $(HDRPATH)appuser.h $(HDRPATH)odbc.h
	$(CC) $(CFLAGS) $(LDLIBS) -c $(SRCPATH)appuser.c -o $(OBJPATH)appuser.o

appfollows.o: $(SRCPATH)appfollows.c $(HDRPATH)appfollows.h $(HDRPATH)odbc.h
	$(CC) $(CFLAGS) $(LDLIBS) -c $(SRCPATH)appfollows.c -o $(OBJPATH)appfollows.o

apptweet.o: $(SRCPATH)apptweet.c $(HDRPATH)apptweet.h $(HDRPATH)odbc.h
	$(CC) $(CFLAGS) $(LDLIBS) -c $(SRCPATH)apptweet.c -o $(OBJPATH)apptweet.o

odbc.o: $(SRCPATH)odbc.c $(HDRPATH)odbc.h
	$(CC) $(CFLAGS) $(LDLIBS) -c $(SRCPATH)odbc.c -o $(OBJPATH)odbc.o

############## USEFUL MAKE COMMANDS

all: clean $(EXE)

clean:
	rm -rf $(EXE) $(OBJPATH)*
