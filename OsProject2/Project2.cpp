//Dylan Guldy
//Operating Systems 
//Project 2
//October 8th 2015

//Include libraries

#include <stdlib.h>		//Exit functions for incorrect files
#include "time_functions.h"	//Timing
#include <string>		//Buffer
#include <fstream>		//Open files
#include <errno.h>		//Debugging
#include <iostream>		//Print for debugging (Remove later?)
#include <pthread.h>		//Threading for program
#include <semaphore.h>		//Semaphores
#include <sstream>		//String buffer
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

double wall_time, cpu_time; //local variables to hold time

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
sem_init(&Full,0, 0);
sem_init(&Empty, 0, 10);
sem_init(&Crit,0,1);

//Timing
start_timing();
cout << "Starting timing: \n";

//Create Producer and Consumer
pthread_t tProducer; pthread_t tConsumer;
pthread_create(&tProducer, NULL, threadProducer,NULL);
pthread_create(&tConsumer, NULL, threadConsumer,NULL);

pthread_join(tProducer, NULL); pthread_join(tConsumer, NULL); 

stop_timing();
wall_time = get_wall_clock_diff();
cpu_time  = get_CPU_time_diff();
cout << "The time it took as wall time is: " << wall_time << ".\n";
cout << "The time it took as CPU time is: " << cpu_time << ".\n";

//Close files, now finished
infile.close(); outfile.close();

//Destory Semaphores
sem_destroy(&Crit); sem_destroy(&Empty); sem_destroy(&Full);
return(0);
}


void *threadConsumer(void * foo){
	cout << "Entering Consumer.\n";
	string temp;
	int index = 0;
	int count = 0;
	temp = Que[index];
	while(temp[0] != char(-1)){	//char 1 was chosen to be a character not in
	sem_wait(&Full);		// the file to signal that its at the end of
	sem_wait(&Crit);		// the program
	temp = Que[index];
	if(temp[0] != char(-1)){
	outfile << temp << "\n";	//Write line
	}
	count++;
	index = (index+1)%10;		//Increment queue, % to make circular
	sem_post(&Crit);		//Release Crit
	sem_post(&Empty);		//Signal empty space
	}
	return (void *)0;
}

void *threadProducer(void * foo){
	cout << "Entering Producer\n";
	string line;
	int count = 0;
	int index = 0;
	
	while(getline(infile, line)){
	sem_wait(&Empty);
	sem_wait(&Crit);		//wait for critical section	
	Que[index] = line;		//Line in queue
	count++;
	index = (index+1)%10;		//Increment queue, % to make circular
	sem_post(&Crit);		//Release Critical for Consumer
	sem_post(&Full);		//Tell Consumer that there is info in queue
	}
	//Done getting lines in file
	sem_wait(&Empty);		//Make sure room is in queue
	Que[index] = char(-1);
	sem_post(&Full);		//Make sure Consumer finishes first
	return (void *)0;
}

