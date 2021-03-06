//Dylan Guldy
//Operating Systems
//Project2
//October 16th 2015

/////////////////////
//Include libraries//
/////////////////////

#include <stdio.h>		//For file input 
#include "time_functions.h"	//For file input
#include <stdlib.h>		//For file input
#include <cstring>		//For file input
#include <fstream>		//File input
#include <pthread.h>		//Threading
#include <vector>		//Setting up arrays
#include <iostream>		//Standard C++ functions
#include <string>		//Use of strings
#include <sstream>		//Parsing of rows

using namespace std;

///////////////////
//Data Structures//
///////////////////

//This is for populating the local array from the array in the file
typedef vector< vector<int> > matrix;

//Defines a matrix, will define input matrix 1 and 2 and output
typedef struct _Matrix {
	matrix m;
	string name;
} Matrix;

//Used by threadMaker to pass data into mult so it knows what to multiply
typedef struct _tempData {
	int row;
	int col;
	int index;
	int id;
}tempData;

/////////////
//Functions//
/////////////

//Prints matrix to out put file and will print to screen as well for trouble shooting
//although I may remove this at the end
void printMatrix(Matrix mat, char const* output);

//Goes through and creates a thread for each location on the product matrix
//then runs the threads
void threadMaker(int thread_count);

//Gets values from threadMaker through the tempData structure, uses input matricies
//to get value for output matrix and puts it in matrix. The idea here is to ensure
//that no two treads can write to the same location at the same time. May need a 
//mutex
void *mult(void * location);

//Opens input file
char const* openInput();

//Opens output file
char const* openOutput();

////////////////////
//Global Variables//
////////////////////
Matrix matt, matty;	//Matt is the first input matrix Matty, his wife is the second
Matrix * matilda;	//The product of Matt and Matty, named Matilda
pthread_mutex_t * lock;


//////////////
//   Main   //
//////////////

int main(int argc, char * argv[])
{

cout << "Start program.\n";

//Number of threads
int threads;

//Get space for product array
matilda = new(Matrix);
//Get space for mutex
lock = new pthread_mutex_t;

//Initialize the mutex
pthread_mutex_init(lock, NULL);

ifstream infile (openInput());

//////////////////////////////////////
//Variables for grabbing out of file//
//////////////////////////////////////
string row;
bool first = true;
int row_num = 0;
int val;
while(!infile.eof())
{					//Sanity check comments, can ignore
getline(infile,row);			//Get line, store in row
  if(row.size() != 0)
  {
    if(row[0] == '*')			//We hit the sperator, reset row, change first
    {
      first = false;			//Now in second array
      row_num = 0;
    }
    else
    {
      stringstream parse(row);		//Split up row into parts
      if(first == true)			//Defines first matrix matt else second matrix matty
      {
        matt.m.push_back(vector<int>()); 
      }
      else
      {
        matty.m.push_back(vector<int>()); 
      }
      while(parse >> val)
      {
        if(first == true)
	{
	  matt.m[row_num].push_back(val);
	}	
	else
	{
	  matty.m[row_num].push_back(val);
	}
      }
      row_num++;			//Next row
    }
  }
}
infile.close();

//Matrices compatable?
if(matt.m[0].size() != matty.m.size())
{
 cout << "The two input matrices are not compadable for mulitplication.\n";
 exit(1);
}

threads = matt.m[0].size() * matt.m.size() * matty.m[0].size();
threadMaker(threads);
//printMatrix(matt, openOutput();
//printMatrix(matty, openOutput();
printMatrix(*matilda, openOutput());

cout << "Done.\n";

return 0;
}


////////////////////////
//Function Definitions//
////////////////////////


void printMatrix(Matrix mat, char const* output)
{
	int i, j;
	ofstream outfile (output);
	outfile << endl;
	for(i=0; i < mat.m.size(); i++)
	{
	  for(j=0; j<mat.m[i].size(); j++)
	  {
		outfile << mat.m[i][j] << "   ";
	  }
		outfile << endl;
	}
	outfile.close();
}

void threadMaker(int thread_count)
{
  int i,j,k;
  int sum = 0;
  int thread_id=0;

  //Array of threads that will run mul function
  pthread_t * multiThread = new pthread_t[thread_count];

  for(i=0; i<matt.m.size(); i++)				//Run through 1st array
  {
    matilda->m.push_back(vector<int>(matty.m[0].size()));	
    for(j=0; j<matty.m[i].size(); j++)				//Run through 2nd array
    {
      for(k=0; k<matt.m[i].size(); k++)				//Product size
      {
        tempData * foo = new tempData;
	foo->row=i;		//row
	foo->col=j;		//col
	foo->index=k;		//index
	foo->id=thread_id;	//ID
	//Create a thread for the mult function and pass in the data given
	pthread_create(&multiThread[thread_id], NULL, mult, (void *)foo);

//	cout << "foo row: " << foo->row << endl;		//debugging print statements
//	cout << "foo col: " << foo->col << endl;
//	cout << "foo index: " << foo->index << endl;
//	cout << "foo id: " << foo->id << endl;
//	cout << "\n";
       	thread_id++;
      }
    }
  }

  //Join and run threads
  for(thread_id=0; thread_id < thread_count; thread_id++)
  {
    pthread_join(multiThread[thread_id], NULL);
  }
  //Free up space again
  delete [] multiThread;
}

void *mult(void * location)
{
  int temp = 0;
  //Make sure only one thread is writing at a time
  pthread_mutex_lock(lock);

  //Put data input into function into the tempData stuct
  tempData * foo = (tempData *) location;
  
  //Multiply values given in by the temporary data holder
  temp = matt.m[foo->row][foo->index] * matty.m[foo->index][foo->col];
  //Store in product matrix
  matilda->m[foo->row][foo->col] += temp;

  //Free up space used
  delete foo;

  //Release lock for next thread
  pthread_mutex_unlock(lock);

  return ((void *)0);
}

//////////////////////////////////////////////////////////////
//Heavily based of provided functions from Doctor Foreman.  //
//Credit goes to him					    //
//////////////////////////////////////////////////////////////
char const* openInput()
{
char aline[50];
char *fullfilepath = (char*) malloc(30);
char *thisprojectfile_in = (char*) malloc(13);
strcpy(thisprojectfile_in,"project3_in.txt");
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
strcpy(thisprojectfile_out,"project3_out.txt");
strcpy(fullfilepath,filebase);
strcat(fullfilepath, thisprojectfile_out);
printf ("fullpath=[%s] \n" , fullfilepath); // for debugging your filename
return fullfilepath;
}
