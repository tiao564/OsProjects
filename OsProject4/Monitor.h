/*Monitor.h The header file*/

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <fstream>

using namespace std;

class Monitor{

public:
	//Constructor function
	Monitor();
	//Destroy function
	~Monitor();
	//Start reading, thread
	void startReading();
	//Stop reading, thread
	void stopReading();
	//Start writing, thread
	void startWriting();
	//Stop writing, thread
	void stopWriting();

	//File writing functions
	void fileRead(string dB, int tID);
	void fileWrite(string db, int tId);

private:
	int activeRead, activeWrite; //These are the active thread counts
	int waitingRead, waitingWrite; //Threads waiting to get into critical section

	ofstream outfile;

	pthread_cond_t OktoRead;  //Thread can read
	pthread_cond_t OktoWrite; //Thread can write
	pthread_mutex_t outFileLock;  //IO file exclusion
	pthread_mutex_t mLock;	  //Exclusion for conditition variables

	/* A multitude of various functions to call easily */
	int notEmpty();
	int isRead();
	int isWrite();
	//Quefunctions will be used for Mode 3
	void lock();
	// Mutex lock
	void fileLock();
	// Lock file
	void unlock();
	// Mutex unlock
	void fileUnlock();
	// File unlock
};
