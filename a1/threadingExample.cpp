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

typedef struct {
	long long ulFrom;
	long long ulTo;
	long long ulResult;
} addstruct;

void* print_message_function( void* ptr );
void cutThread(pthread_t* thread, addstruct* params);
void joinAll(pthread_t* threads, int iThreadCount);
long long sumAll(addstruct* params, int iThreadCount);
void runAllThreads(pthread_t* threads, addstruct* params, int iThreadCount);
void setupParams(addstruct params[], int iThreadCount, long long ulMax, long long ulSectionSize);
void assertOrDie(bool assertion, string sMsg);

int main(int argc, char* argv[]) {
    pthread_t* threads;
	addstruct* params;
	long long ulTotal = 0;
	long long ulMax;
	long long ulSectionSize;
	int iThreadCount;
	
	assertOrDie(argc == 3, "Usage: sum [max_num] [# of threads]");
	
	ulMax = atol(argv[1]);
	iThreadCount = atoi(argv[2]);
	
	assertOrDie(ulMax >= iThreadCount, "Thread count cannot be larger than the maximum number to sum to");
	assertOrDie(ulMax > 0, "max_num must be above 0");
	assertOrDie(iThreadCount > 0, "# of threads must be above 0");
	
	ulSectionSize = 1 + ((ulMax - 1) / iThreadCount); //Divide and round up

	threads = new pthread_t[iThreadCount];
	params = new addstruct[iThreadCount];
	
	setupParams(params, iThreadCount, ulMax, ulSectionSize);
	assertOrDie(params[0].ulFrom == 1, "First thread won't start at 0, something is wrong");
	runAllThreads(threads, params, iThreadCount);
	
	joinAll(threads, iThreadCount);
	
	cout << "Sum(1, " << ulMax << ") = " << sumAll(params, iThreadCount) << endl;

    return 0;
}

void assertOrDie(bool assertion, string sMsg){
	if (!assertion) {
		cout << sMsg << endl;
		exit(-1);
	}
}

void* sumFunction( void* ptr )
{
    addstruct *as;
    as = (addstruct *) ptr;
	as->ulResult = 0;
	for (long long i = as->ulFrom; i <= as->ulTo; i++)
		as->ulResult += i;
	return 0;
}

void setupParams(addstruct params[], int iThreadCount, long long ulMax, long long ulSectionSize) {
	if (ulMax / iThreadCount < 2) {
		for (int i = 0; i < iThreadCount; i++) {
			params[i].ulFrom = i + 1;
			if (i == iThreadCount - 1) {
				params[i].ulTo = ulMax;
			} else {
				params[i].ulTo = i + 1;
			}
		}
	} else {
		for (int i = 0; i < iThreadCount; i++) {
			params[i].ulFrom = ulSectionSize * i + 1;
			if (i == iThreadCount - 1) {
				params[i].ulTo = ulMax;
			} else {
				params[i].ulTo = params[i].ulFrom + ulSectionSize - 1;
			}
		}
	}
}

void runAllThreads(pthread_t* threads, addstruct* params, int iThreadCount) {
	for (int i = 0; i < iThreadCount; i++) {
		cutThread(&threads[i], &params[i]);
	}
}

void cutThread(pthread_t* thread, addstruct* params) {
	pthread_create(thread, NULL, sumFunction, params);
	cout << "Thread " << *thread << ": (" << params->ulFrom << ", " << params->ulTo << ")" << endl;
}

void joinAll(pthread_t* threads, int iThreadCount) {
	for (int i = 0; i < iThreadCount; i++) {
		pthread_join( threads[i], NULL);
	}
}

long long sumAll(addstruct* params, int iThreadCount) {
	long long ulSum = 0;
	for (int i = 0; i < iThreadCount; i++) {
		ulSum += params[i].ulResult;
	}
	return ulSum;
}
