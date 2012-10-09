/**********************************
Last Name: Ludwig
First Name: Mitchell
Student ID: 10015370
Course: CPSC 457
Tutorial Section: T02
Assignment: 1
Question: P1Q1

File name: sum.cpp 
**********************************/

#include <pthread.h>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

//Used to pass data between threads
typedef struct {
	long long ulFrom;   //Start summing from this number
	long long ulTo;     //Sum to (inclusive) this number
	long long ulResult; //Store the result here
} sumData;


//Test some things
void testSuite();
//Threads run this function to sum up their range
void* sumFunction( void* ptr );
//Create a thread and pass in the range
void cutThread(pthread_t* thread, sumData* params);
//Wait until all threads are finished executing
void joinAll(pthread_t* threads, int iThreadCount);
//Sum the results from each thread
long long sumAll(sumData* params, int iThreadCount);
//Run all threads
void runAllThreads(pthread_t* threads, sumData* params, int iThreadCount);
//Set up the structures holding the sum ranges
void setupParams(sumData params[], int iThreadCount, long long ulMax);
//if (assertion == false) then end the program
void assertOrDie(bool assertion, string sMsg);

int main(int argc, char* argv[]) {
	pthread_t* threads;   //Thread array
	sumData* params;	  //Sum range parameter array
	long long ulMax;	  //Number to sum up to
	int iThreadCount;	  //Number of threads summing
	
	//Run some tests to make sure I'm a good programmer 
	testSuite();
	
	//Ensure that the correct number of arguments are being used
	assertOrDie(argc == 3, "Usage: sum [max_num] [# of threads]");
	
	//Get the maximum number and the number of threads
	ulMax = atol(argv[1]);
	iThreadCount = atoi(argv[2]);
	
	//Ensure that the program is being used correctly
	assertOrDie(ulMax >= iThreadCount, "Thread count cannot be larger than the maximum number to sum to");
	assertOrDie(ulMax > 0, "max_num must be above 0");
	assertOrDie(iThreadCount > 0, "# of threads must be above 0");
	
	//Initialize the thread and parameter arrays
	threads = new pthread_t[iThreadCount];
	params = new sumData[iThreadCount];
	
	//Setup the sum ranges to pass into the program
	setupParams(params, iThreadCount, ulMax);
	//Run all of the threads with the sum ranges in params[]
	runAllThreads(threads, params, iThreadCount);
	//Wait until each thread completes
	joinAll(threads, iThreadCount);
	//Output the result
	cout << "Sum(1, " << ulMax << ") = " << sumAll(params, iThreadCount) << endl;
	//Die
    return 0;
}

//Tests sumFunction and setupParams
void testSuite() {
	sumData param;
	param.ulFrom = 1;
	param.ulTo = 5;
	sumFunction(&param);
	assertOrDie(param.ulResult == 15, "sumFunction incorrect sum");
	param.ulFrom = 1;
	param.ulTo = 1;
	sumFunction(&param);
	assertOrDie(param.ulResult == 1, "sumFunction incorrect sum");
	param.ulFrom = 1;
	param.ulTo = 10;
	sumFunction(&param);
	assertOrDie(param.ulResult == 55, "sumFunction incorrect sum");
	param.ulFrom = 2;
	param.ulTo = 3;
	sumFunction(&param);
	assertOrDie(param.ulResult == 5, "sumFunction incorrect sum");

	sumData* params;
	params = new sumData[1];
	setupParams(params, 1, 1);
	assertOrDie(params[0].ulFrom == 1 && params[0].ulTo == 1, "setupParams incorrect setup");

	params = new sumData[1];
	setupParams(params, 1, 5);
	assertOrDie(params[0].ulFrom == 1 && params[0].ulTo == 5, "setupParams incorrect setup");

	params = new sumData[2];
	setupParams(params, 2, 10);
	assertOrDie(params[0].ulFrom == 1 && params[0].ulTo == 5, "setupParams incorrect setup");
	assertOrDie(params[1].ulFrom == 6 && params[1].ulTo == 10, "setupParams incorrect setup");
	
	params = new sumData[4];
	setupParams(params, 4, 15);
	assertOrDie(params[0].ulFrom == 1 && params[0].ulTo == 4, "setupParams incorrect setup");
	assertOrDie(params[1].ulFrom == 5 && params[1].ulTo == 8, "setupParams incorrect setup");
	assertOrDie(params[3].ulFrom == 13 && params[3].ulTo == 15, "setupParams incorrect setup");
	
	params = new sumData[5];
	setupParams(params, 5, 11);
	assertOrDie(params[0].ulFrom == 1 && params[0].ulTo == 2, "setupParams incorrect setup");
	assertOrDie(params[1].ulFrom == 3 && params[1].ulTo == 4, "setupParams incorrect setup");
	assertOrDie(params[4].ulFrom == 9 && params[4].ulTo == 11, "setupParams incorrect setup");
}

//Handy function to shorthand some error checking and input validation
void assertOrDie(bool assertion, string sMsg){
	if (!assertion) {
		cout << sMsg << endl;
		exit(-1);
	}
}

//Sum the given range, pointed to by ptr, and store the result in the structure pointed to by ptr
void* sumFunction( void* ptr )
{
	//Setup the pointer to access the data
    sumData *as;
    as = (sumData *) ptr;
	//Initialize the resulting sum to 0
	as->ulResult = 0;
	//For each number from ulFrom to ulTo (inclusive) sum the number into the result
	for (long long i = as->ulFrom; i <= as->ulTo; i++)
		as->ulResult += i;
	return 0;
}

//Setup the summing ranges for each thread
//This could have been implemented inside each thread, but for simplicity was placed outside
void setupParams(sumData params[], int iThreadCount, long long ulMax) {
	long long ulSectionSize;  //Number of numbers each thread will sum
	
	//If each thread can have 2
	//The section size attempts even out the section sizes
	//With t threads, the first t-1 threads will calculate evenly, with a size of ulSectionSize
	//The final thread must calculate the sum of at least one number
	//The sad end result of this poor optimization is easily demonstrated by t=7, n=11
	//Thread handling:  12345677777
	//Number Range:     123456789AB
	//You can see thread 7 doing 5x the work of other threads, but that's in the assignment
	// requirements, so that's how it's implemented.
	if (iThreadCount == 1) {
		//If there's only one thread, don't try division by (iThreadCount - 1)
		params[0].ulFrom = 1;
		params[0].ulTo = ulMax;
	} else {
		if (ulMax % iThreadCount == 0) {
			//If it's cleanly divisible, be happy!
			ulSectionSize = ulMax / iThreadCount;
		} else {
			//Otherwise, use this awkward function to satisfy the assignment requirements
			//This could really be optimized but hey, all this time we haven't
			//been discussing sumTo(n) = n(n+1)/2, so this can slide.
			ulSectionSize = (ulMax - 1) / (iThreadCount - 1);
		}
		//Simply loop through each thread and assign the correct range
		for (int i = 0; i < iThreadCount; i++) {
			params[i].ulFrom = ulSectionSize * i + 1;
			if (i == iThreadCount - 1) {
				//With an exception for the last thread, which gets the dregs
				params[i].ulTo = ulMax;
			} else {
				params[i].ulTo = params[i].ulFrom + ulSectionSize - 1;
			}
		}
	}
}

//Runs each thread
void runAllThreads(pthread_t* threads, sumData* params, int iThreadCount) {
	for (int i = 0; i < iThreadCount; i++) {
		cutThread(&threads[i], &params[i]);
	}
}

//Runs a single thread with the range inside *params
void cutThread(pthread_t* thread, sumData* params) {
	pthread_create(thread, NULL, sumFunction, params);
	cout << "Thread " << *thread << ": (" << params->ulFrom << ", " << params->ulTo << ")" << endl;
}

//Waits for all threads to finish
void joinAll(pthread_t* threads, int iThreadCount) {
	for (int i = 0; i < iThreadCount; i++) {
		pthread_join( threads[i], NULL);
	}
}

//Adds up each thread's results
long long sumAll(sumData* params, int iThreadCount) {
	long long ulSum = 0;
	for (int i = 0; i < iThreadCount; i++) {
		ulSum += params[i].ulResult;
	}
	return ulSum;
}
