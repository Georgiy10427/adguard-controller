CC=g++
CFLAGS=-Wall -std=c++17 -pthread -lboost_filesystem -lboost_thread -lboost_system -O3 
CROSS_CFLAGS=-Wall -pthread -std=c++17 -mcpu=cortex-a35
FILE=web_server.cpp

all:
	clear
	# Build app:
	########################
	$(CC) $(FILE) $(CFLAGS) -o server.bin
	########################
	./server.bin

create-profile:
	$(CC) $(FILE) $(CFLAGS) -fprofile-generate -o server.bin

use-profile:
	$(CC) $(FILE) $(CFLAGS) -fprofile-use -o server.bin

cross:
	aarch64-linux-gnu-g++ $(FILE) $(CROSS_CFLAGS) -O3 -o server_arm.bin

github-action:
	$(CC) $(FILE) $(CFLAGS) -o server.bin
