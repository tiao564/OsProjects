/*Monitor.h The header file*/

#include <pthred.h>
#include <iostream>
#include "time_functions.h"
#include <fstresm>
#include <queue>

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
	//Mode set, defines mode set by instructor
	void setMode(int bar);

	//File writing functions
	void fileRead(string dB, int tID);
	void fileWrite(string db, int tId);


private:
	int mode;

	int activeRead, activeWrite; //These are the active thread counts
	int waitingRead, waitingWrite; //Threads waiting to get into critical section

	queue<int> FCFS; //for Mode 3, first come first serve

	pthread_cond_t OktoRead;  //Thread can read
	pthread_cond_t OktoWrite; //Thread can write
	pthread_mutex_t outFile;  //IO file exclusion
	pthread_mutex_t mLock;	  //Exclusion for conditition variables

	/* A multitude of various functions to call easily */
	void pop();
	void pushReader();
	void pushWriter();
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
	void waitWrite();
	void waitRead();
	// Protects condition variable for decrement 
	void broadcastRead();
	// signal all readers
};
