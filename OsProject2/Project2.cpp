//Dylan Guldy
//Operating Systems 
//Project 2
//October 8th 2015

//Include libraries

#include <stdlib.h>	//Exit functions for incorrect files
#include <time.h>	//Timing
#include <string>	//Buffer
#include <fstream>	//Open files
#include <errno.h>	//Debugging
#include <iostream>	//Print for debugging (Remove later?)
#include <pthread.h>	//Threading for program
#include <semaphore.h>	//Semaphores
#include <sstream>	//String buffer
using namespace std;

//Thread functions
void *threadProducer(void * foo);

void *threadConsumer(void * foo);



//Global variables

int debug = 0;			//Will be used to signal a problem and print statments accordingly. Remove when done?
sem_t Crit, Full, Empty;	//Semaphores
					//signals for full, empty and critical sections
ifstream infile;		//In file
ofstream outfile;		//Out file
string Que[10];			//Circular queue


//            MAIN
//Main will handle taking in and opening files.
//	It will also create and run both threads as
//	well as handle timing how long the threads take
//	to run.
int main(int argc, char * argv[]){


//Open input file
infile.open(argv[1]);
if(!infile.good()){
	cerr << "The input file name was not correct. Please try again.\n";
	exit(0);
}

outfile.open(argv[2]);
if(!outfile.good()){
	cerr << "The output file name was not correct. Please try again.\n";
	exit(0);
}

//Init Semaphores
int v1, v2, v3;
sem_init(&Full,0, 0);
sem_init(&Empty, 0, 10);
sem_init(&Crit,0,1);


clock_t cpuStart, cpuEnd;
stuct timeb wStart, wEnd;
cout << "Starting timing: \n";
ftime(&wStart); cpuStart = clock();

return(0);
}


void threadConsumer(void * foo){

}

void threadProducer(void * foo){

}
