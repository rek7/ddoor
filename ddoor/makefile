OBJS	= dns_query.o process_commands.o crypto.o doh_query.o main.o startup.o
SOURCE	= dns_query.cpp process_commands.cpp crypto.cpp doh_query.cpp main.cpp startup.cpp
HEADER	= config.h startup.h dns_query.h doh_query.h libhttp.h process_commands.h crypto.h
OUT	= ../bin/binary
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread -lresolv

all: $(OBJS)
		$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

dns_query.o: dns_query.cpp
		$(CC) $(FLAGS) dns_query.cpp -std=c++11 -lcunit

process_commands.o: process_commands.cpp
		$(CC) $(FLAGS) process_commands.cpp -std=c++11 -lcunit

crypto.o: crypto.cpp
		$(CC) $(FLAGS) crypto.cpp -std=c++11 -lcunit

doh_query.o: doh_query.cpp
		$(CC) $(FLAGS) doh_query.cpp -std=c++11 -lcunit

main.o: main.cpp
		$(CC) $(FLAGS) main.cpp -std=c++11 -lcunit

startup.o: startup.cpp
		$(CC) $(FLAGS) startup.cpp -std=c++11 -lcunit


clean:
		rm -f $(OBJS) $(OUT)
