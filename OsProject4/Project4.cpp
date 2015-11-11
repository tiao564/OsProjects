//Dylan Guldy
//Operating Systems
//Project 4 Read and write with Monitors
//November 10th 2015


/*Main*/

#include "time_functions.h"
#include "Monitor.h"
#include <string>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <fstream>

using namespace std;

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
const char * openOutput();

void currentTime;
////////////////////
//Global variables//
////////////////////

string dB;

const char *  projectfilein = openInput();
const char *  projectfileout = openOutput();
//Monitor ReadWrite;

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
  int W = atoi(&inputs[4]);
  cout << "Num readers: " << r << "\nNum writers: " << w << "\nReaders delay: " << R;
  cout << "\nWriters delay: " << W << endl;


  pthread_t * tReader = new pthread_t[r];
  pthread_t * tWriter = new pthread_t[w];
  int tR, tW;

  dB = currentTime();

  for(tW=0; tW<2; tW++){
  tData * bar = new tData;
  bar->tID = tW;
  bar->tDelay=W;
  pthread_create(&tWriter[tW], NULL, threadWrite, (void *)bar);
  }
  for(tR=0; tR<r; tR++){
  tData * bar = new tData;
  bar->tID = tR;
  bar->tDelay= R;
  pthread_create(&tReader[tR], NULL, threadRead, (void *)bar);
  }

  for(tW=0; tW<w; tW++){
  pthread_join(tWriter[tW], NULL);
  }
  
  for(tR=0; tR<w; tR++){
  pthread_join(tReader[tR], NULL);
  }

  delete [] tReader;
  delete [] tWriter;

  return 0;
}

void *threadRead(void * foo){
  int j;
  string dBRead;
  tData * bar = (tData *)foo;  // Cast to new struct
  int iter;
  
  for(iter=0; iter<10; iter++){
  ReadWrite.startReading();

  dBRead = dB;

  ReadWrite.fileRead(dBRead, bar->tID);

  ReadWrite.stopReading();

  for(j=0;j<(bar -> tDelay); j++){}
  }
 delete bar;

 return (void *)0; 
}

void *threadWrite(void * bar){
  int k;
  tData * foo = (tData *)bar; //Cast to new struct
  int iter; 
  for(iter=0; iter<10;iter++)
  {
    ReadWrite.startWriting();

      dB = currentTime();
      dB = "dickbutt";
    ReadWrite.fileWrite(dB, foo-> tID);

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

char const* openOutput()
{
char aline[50];
char *fullfilepath = (char*) malloc(30);
char *thisprojectfile_out = (char*) malloc(13);
strcpy(thisprojectfile_out,"project4_out.txt");
strcpy(fullfilepath,filebase);
strcat(fullfilepath, thisprojectfile_out);
printf ("fullpath=[%s] \n" , fullfilepath); // for debugging your filename
return fullfilepath;
}

string currentTime(){
  int seconds, millisecs;
  string timing;
  string secnds, mil;
  get_wall_time_ints(&seconds, &millisecs);
  secnds = to_string(seconds);
  mil = to_string(millisecs);
  timing = secnds + " " + mil; 
  return timing;
}

