//Dylan Guldy
//Operating Systems
//Project 4 Read and write with Monitors
//November 10th 2015


/*Main*/

#include <vector>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include "pthread.h"
#include "time_functions.h"

using namespace std;

class Monitor{
private:	
	int activeRead, activeWrite; //These are the active thread counts
	int waitingRead, waitingWrite; //Threads waiting to get into critical section

	ofstream outfile;

	pthread_cond_t OktoRead;  //Thread can read
	pthread_cond_t OktoWrite; //Thread can write
	pthread_mutex_t outFileLock;  //IO file exclusion
	pthread_mutex_t mLock;	  //Exclusion for conditition variables

	/* A multitude of various functions to call easily */
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
	void fileRead(int time, int mm, int tID);
	void fileWrite(int time, int mm, int tID);
};
////////////////////////
//Structure for thread//
////////////////////////
typedef struct _tData{
  int tID;
  int tDelay;
} tData;


/////////////
//Functions//
/////////////

//The monoitor is called by these functions and will ensure variable protection
void *threadRead(void * foo);

void *threadWrite(void * bar);
const char * openInput();

////////////////////
//Global variables//
////////////////////

string dB;

const char *  projectfilein = openInput();
int timeS;
int timeMS;
Monitor ReadWrite;

int main(){
  //Open file and get inputs for running//
  string inputs;
  ifstream infile;
  infile.open(projectfilein);
  getline(infile,inputs);
  stringstream parse(inputs);
  int r = atoi(&inputs[0]);
  int w = atoi(&inputs[1]);
  int R = atoi(&inputs[3]);
  int W = atoi(&inputs[12]);
  cout << "Num readers: " << r << "\nNum writers: " << w << "\nReaders delay: " << R;
  cout << "\nWriters delay: " << W << endl;

  //Declare threads and thread variables
  pthread_t * tReader = new pthread_t[r];
  pthread_t * tWriter = new pthread_t[w];
  int tR, tW;

  //Create writer threads
  for(tW=0; tW<2; tW++){
  tData * bar = new tData;
  bar->tID = tW;
  bar->tDelay=W;
  pthread_create(&tWriter[tW], NULL, threadWrite, (void *)bar);
  }

  //Create reader threads
  for(tR=0; tR<r; tR++){
  tData * bar = new tData;
  bar->tID = tR;
  bar->tDelay= R;
  pthread_create(&tReader[tR], NULL, threadRead, (void *)bar);
  }

  //Join writers
  for(tW=0; tW<w; tW++){
  pthread_join(tWriter[tW], NULL);
  }
  
  //Join readers
  for(tR=0; tR<w; tR++){
  pthread_join(tReader[tR], NULL);
  }

  //free space
  delete [] tReader;
  delete [] tWriter;

  return 0;
}

  //Call monitor and run threads using a temp data struct (reading)
void *threadRead(void * foo){
  int j;
  string dBRead;
  tData * bar = (tData *)foo;  // Cast to new struct
  int iter;
  
  for(iter=0; iter<10; iter++){
  ReadWrite.startReading();

  dBRead = dB;
  get_wall_time_ints(&timeS, &timeMS);
  ReadWrite.fileRead(timeS, timeMS, bar->tID);
  ReadWrite.stopReading();

  for(j=0;j<(bar -> tDelay); j++){}
  }
 delete bar;

 return (void *)0; 
}

  //Call monitor and run threads using a temp data structure (writing)
void *threadWrite(void * bar){
  int k;
  tData * foo = (tData *)bar; //Cast to new struct
  int iter; 
  for(iter=0; iter<10;iter++)
  {
    ReadWrite.startWriting();

      ReadWrite.fileWrite(timeS, timeMS, foo-> tID);

    for(k=0; k< (foo->tDelay);k++){}
  }
  delete foo;
}

////////////////////////////////////////////////////////////
//Heavily based of provided functions from Doctor Foreman.  //
//Credit goes to him              //
//////////////////////////////////////////////////////////////
char const* openInput()
{
char aline[50];
char *fullfilepath = (char*) malloc(30);
char *thisprojectfile_in = (char*) malloc(13);
strcpy(thisprojectfile_in,"project4_in.txt");
strcpy(fullfilepath,filebase);
strcat(fullfilepath, thisprojectfile_in);
printf ("fullpath=[%s] \n" , fullfilepath); // for debugging your filename
return fullfilepath;
}


//Initalize Monitor
Monitor::Monitor(){
  pthread_cond_init(&OktoRead,NULL);
  pthread_cond_init(&OktoWrite,NULL);
  pthread_mutex_init(&mLock, NULL);
  pthread_mutex_init(&outFileLock, NULL);

  char *fullfilepath = (char*) malloc(30);
  char *thisprojectfile_out = (char*) malloc(13);
  strcpy(thisprojectfile_out,"project4_out.txt");
  strcpy(fullfilepath,filebase);
  strcat(fullfilepath, thisprojectfile_out);

  outfile.open(fullfilepath);

  activeRead=0;
  activeWrite=0;
  waitingRead=0;
  waitingWrite=0;
}

//Destory Monitor
Monitor::~Monitor(){
  pthread_cond_destroy(&OktoRead);
  pthread_cond_destroy(&OktoWrite);
  pthread_mutex_destroy(&mLock);
  pthread_mutex_destroy(&outFileLock);

  outfile.close();
}
// See if reading can begin
void Monitor::startReading(){
  lock();
    while((activeWrite + waitingWrite)>0){   //If writing or if waiting to write hold
     waitingRead++;
     pthread_cond_wait(&OktoRead, &mLock);
     waitingRead--;
    }
  unlock();
}

//Stop reading and signal ok to write
void Monitor::stopReading(){
 lock();
  activeRead--;
  if (activeRead == 0 && waitingWrite > 0){
    pthread_cond_signal(&OktoWrite);
  }
  unlock();
}

//See if its ok to write
void Monitor::startWriting() {
  lock();
     while ((activeWrite + activeRead) > 0) {
        waitingWrite++;
        pthread_cond_wait(&OktoWrite, &mLock);
        waitingWrite--;
     }
  unlock();
}

//stop writing and signal ok to read
void Monitor::stopWriting(){
  lock();
   activeWrite--;
    if (waitingWrite > 0) {
      pthread_cond_signal(&OktoWrite);
    }
    else if (waitingRead > 0) {
      pthread_cond_broadcast(&OktoRead);
    }
  unlock();
  }

//File IO
void Monitor::fileRead(int time, int mm, int tID){
  fileLock();
  outfile << ">>> DB value set to: " << time << ":" << mm << " by reader thread numaber: " << tID << endl;
  fileUnlock(); 
}

void Monitor::fileWrite(int time, int mm, int tID){
  fileLock();
  outfile << ">>> DB value set to: " << time << ":" << mm << " by writer thread numaber: " << tID << endl;
  fileUnlock();
}

//Private
//Functions simply lock and unlock corrosponding mutexs
void Monitor::lock(){
  pthread_mutex_lock(&mLock);
}

void Monitor::fileLock(){
  pthread_mutex_lock(&outFileLock);
}

void Monitor::unlock(){
  pthread_mutex_unlock(&mLock);
}

void Monitor::fileUnlock(){
  pthread_mutex_unlock(&outFileLock);
}

