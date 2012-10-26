#include <pthread.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

//User inputted data
int iBufSize;
int iBufCount;
int iProdCount;
int iConsCount;
int iItemsPerProd;
//Counter of how many items still need consuming
int iItemsLeft;

//Do tests?
bool bTest;

//Buffers (array of bytes [segmented by an accessor function])
char* baBuffers;
//BufferPointers (arrays of ints)
int* iaBufIn;
int* iaBufOut;
//IdPointers (simply store thread IDs)
int* iaIDs;
//Buffer mutexes
pthread_mutex_t* maBuf;
pthread_mutex_t mPrinting;
pthread_mutex_t mCountdown;

//Producer and consumer threads
pthread_t* ptProducers;   //Thread array
pthread_t* ptConsumers;   //Thread array

/*
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
*/
//Algorithm for a single consumer
void* consumer( void* ptr );
//Algorithm for a single producer
void* producer( void* ptr );
//Returns true if the buffer is full
bool isFull(int iBufferIndex);
//Returns true if the buffer is empty
bool isEmpty(int iBufferIndex);
//Pushes a value onto the top of a buffer
//returns true if it succeeds, and false if the buffer is full
bool pushBufVal(int iBufferIndex, char value, int iID);
//Pops a value off the bottom of a buffer
//returns the value on success and -1 if the buffer is empty
char popBufVal(int iBufferIndex, int iID);
//Initialize the buffers
void initBuffers();
//Get specific value in specific buffer
char getBufVal(int iBufferIndex, int iBufferOffset);
//Set specific value in specific buffer
void setBufVal(int iBufferIndex, int iBufferOffset, char value);
//Prints an exiting thread
void printExit(int iID, bool bIsProd);
//Prints the contents of a buffer directly after a production or a consumption
//NOTE: Assumes there is a present lock on the buffer from the calling method
void printBuffer(int iID, bool bIsProd, int iBuffer);
//Test some things
void testSuite();
//If (assertion == false) then end the program
void assertOrDie(bool assertion, string sMsg);
//Get data from the user
void getUserInput(int argc, char* argv[]);
//Generate a semirandom number
int semiRandom();
//Returns true if all items have been consumed
bool allDone();
//Subtracts one from the countdown, but is mutexed. In reality, on an x86 architecture, decrementing is an atomic operation, but whatever
void countdown();
//Runs all producers and consumers
void cutAll();
//Waits for all threads to finish
void joinAll();
int main(int argc, char* argv[]) {
    //Get the command line input
    getUserInput(argc, argv);
    
    //Initialize the buffers
    initBuffers();
    
    //Run some tests to make sure I'm a good programmer 
    testSuite();
    
    ptProducers = new pthread_t[iProdCount];
    ptConsumers = new pthread_t[iConsCount];
    
	cutAll();

    joinAll();
    return 0;
}

//Consumer algorithm
void* consumer( void* ptr ) {
    int iID = *((int *) ptr);
    bool bAllEmpty;
	char cDebug;
    
    while(!allDone()) {
        bAllEmpty = true;
        for (int iBufIndex = 0; iBufIndex < iBufCount; iBufIndex++) {
			cDebug = popBufVal(iBufIndex, iID);
			if (cDebug  != -1) {
                bAllEmpty = false;
                countdown();
				sleep(1);
			    break;
            }
        }
        if (bAllEmpty) {
            sleep((rand() % 4) + 1);
        }
    }
    printExit(iID, false);
    return 0;
}

//Producer algorithm
void* producer( void* ptr ) {
    int iID = *((int *) ptr);
    bool bAllFull;
    
    for (int i = 0; i < iItemsPerProd; i++) {
        bAllFull = true;
        for (int iBufIndex = 0; iBufIndex < iBufCount; iBufIndex++) {
            if (pushBufVal(iBufIndex, semiRandom(), iID)) {
                bAllFull = false;
                break;
            }
        }
        if (bAllFull) {
            sleep((rand() % 2) + 1);
            i--;
        }
    }
    printExit(iID, true);
    return 0;
}

//Pushes a value onto the top of a buffer
//returns true if it succeeds, and false if the buffer is full
bool pushBufVal(int iBufferIndex, char value, int iID) {
    pthread_mutex_lock(&maBuf[iBufferIndex]);
    if (isFull(iBufferIndex)) {
        pthread_mutex_unlock(&maBuf[iBufferIndex]);
        return false;
    }
    setBufVal(iBufferIndex, iaBufIn[iBufferIndex], value);
    iaBufIn[iBufferIndex] = (iaBufIn[iBufferIndex] + 1) % (iBufSize+1);
    printBuffer(iID, true, iBufferIndex);
    pthread_mutex_unlock(&maBuf[iBufferIndex]);
    return true;
}

//Pops a value off the bottom of a buffer
//returns the value on success and -1 if the buffer is empty
char popBufVal(int iBufferIndex, int iID) {
    pthread_mutex_lock(&maBuf[iBufferIndex]);
    char cVal;
    if (isEmpty(iBufferIndex)) {
        pthread_mutex_unlock(&maBuf[iBufferIndex]);
        return -1;
    }
    cVal = getBufVal(iBufferIndex, iaBufOut[iBufferIndex]);
    iaBufOut[iBufferIndex] = (iaBufOut[iBufferIndex] + 1) % (iBufSize+1);
    printBuffer(iID, false, iBufferIndex);
    pthread_mutex_unlock(&maBuf[iBufferIndex]);
    return cVal;
}

//Returns true if the buffer is empty
bool isEmpty(int iBufferIndex) {
    return iaBufIn[iBufferIndex] == iaBufOut[iBufferIndex];
}

//Returns true if the buffer is full
bool isFull(int iBufferIndex) {
    return ((iaBufIn[iBufferIndex] + 1) % (iBufSize+1)) == iaBufOut[iBufferIndex];
}

//Set specific value in specific buffer
void setBufVal(int iBufferIndex, int iBufferOffset, char value) {
    baBuffers[iBufferIndex * (iBufSize+1) + iBufferOffset] = value;
}

//Get specific value in specific buffer
char getBufVal(int iBufferIndex, int iBufferOffset) {
    return baBuffers[iBufferIndex * (iBufSize+1) + iBufferOffset];
}
//Initializes the buffers
void initBuffers() {
    baBuffers = new char[iBufCount*(iBufSize+1)];
    iaBufIn = new int[iBufCount];
    iaBufOut = new int[iBufCount];
    maBuf = new pthread_mutex_t[iBufCount];
	iaIDs = new int[max(iProdCount,iConsCount)];
    for (int i = 0; i < iBufCount; i++) {
        pthread_mutex_init(&maBuf[i], NULL);
    }
	for (int i = 0; i < max(iProdCount,iConsCount); i++) {
		iaIDs[i] = i;
	}
    pthread_mutex_init(&mPrinting, NULL);
    pthread_mutex_init(&mCountdown, NULL);
}

//Returns true if all items have been consumed
bool allDone() {
    return iItemsLeft < 1;
}

//Subtracts one from the countdown, but is mutexed. In reality, on an x86 architecture, decrementing is an atomic operation, but whatever
void countdown() {
    pthread_mutex_lock(&mCountdown);
    iItemsLeft--;
    pthread_mutex_unlock(&mCountdown);
}

//Prints an exiting thread
void printExit(int iID, bool bIsProd) {
    pthread_mutex_lock(&mPrinting);
    if (bIsProd) {
        cout << "Producer " << iID << " is exiting" << endl;
    } else {
        cout << "Consumer " << iID << " is exiting." << endl;
    }
    pthread_mutex_unlock(&mPrinting);
}

//Prints the contents of a buffer directly after a production or a consumption
//NOTE: Assumes there is a present lock on the buffer from the calling method
void printBuffer(int iID, bool bIsProd, int iBuffer) {
    pthread_mutex_lock(&mPrinting);
    if (bIsProd) {
        cout << "Producer " << iID << ":  B" << iBuffer << ":";
    } else {
        cout << "Consumer " << iID << ":  B" << iBuffer << ":";
    }
    for (int i = iaBufOut[iBuffer]; i != iaBufIn[iBuffer]; i = (i+1) % (iBufSize + 1)){
        cout << (int)getBufVal(iBuffer, i) << " ";
    }
    cout << endl;
    pthread_mutex_unlock(&mPrinting);
}

//Obtains input from the user
void getUserInput(int argc, char* argv[]) {
    //Ensure that the correct number of arguments are being used
	if (argc == 7) {
		bTest = true;
	} else {
		assertOrDie(argc == 6, "Usage: pc [BufferSize] [BufferCount] [ProducerCount] [ConsumerCount] [ItemsPerProducer]");
		bTest = false;
    }
	
    //Get the user's input data 
    iBufSize = atoi(argv[1]);
    iBufCount = atoi(argv[2]);
    iProdCount = atoi(argv[3]);
    iConsCount = atoi(argv[4]);
    iItemsPerProd = atoi(argv[5]);
    
    //Ensure that the program is being used correctly
    assertOrDie(iBufSize > 0, "The buffer size must be at least 1");
    assertOrDie(iBufCount > 0, "The number of buffers must be at least 1");
    assertOrDie(iProdCount > 0, "The number of producers must be at least 1");
    assertOrDie(iConsCount > 0, "The number of consumers must be at least 1");
    assertOrDie(iItemsPerProd > 0, "The number of items per producer must be at least 1");
    
    iItemsLeft = iItemsPerProd * iProdCount;
}

//Handy function to shorthand some error checking and input validation
void assertOrDie(bool assertion, string sMsg){
    if (!assertion) {
        cout << sMsg << endl;
        exit(-1);
    }
}

//Tests sumFunction and setupParams
void testSuite() {
	if (!bTest) return;
    for (int i = 0; i < iBufCount; i++) {
        for (int i2 = 0; i2 < iBufSize; i2++) {
            assertOrDie(getBufVal(i,i2) == 0, "Array not initialized properly");
        }
        assertOrDie(iaBufIn[i] == 0, "Array not initialized properly");
        assertOrDie(iaBufOut[i] == 0, "Array not initialized properly");
    }
    for (int i = 0; i < iBufCount; i++) {
        for (int i2 = 0; i2 < iBufSize; i2++) {
            assertOrDie(pushBufVal(i,(char)((i * 31 + i2) % 99),1), "Buffer failure");
        }
        assertOrDie(isFull(i), "Buffer failure");
        assertOrDie(!pushBufVal(i,12,1) , "Buffer failure");
        for (int i2 = 0; i2 < iBufSize; i2++) {
            assertOrDie(popBufVal(i,1) == (char)((i * 31 + i2) % 99), "Buffer failure");
        }
        assertOrDie(isEmpty(i), "Buffer failure");
        pthread_mutex_lock(&maBuf[i]);
        assertOrDie(pthread_mutex_trylock(&maBuf[i]) != 0, "Mutex failure");
        pthread_mutex_unlock(&maBuf[i]);
        assertOrDie(pthread_mutex_trylock(&maBuf[i]) == 0, "Mutex failure");
        pthread_mutex_unlock(&maBuf[i]);
    }
}

//Predictable pseudorandom number generator (useful while testing)
int semiRandom() {
    static int iRand = 7;
    static int iRand2 = 3;
    static int iAdd = 1;
    iRand = (iRand * iRand) % 100;
    iRand2 = ((iRand ^ iRand2) * 79);
    iRand2 += iAdd++;
    iRand2 = ((iRand2 + 17) ^ iRand) % 100;
    return iRand2 ^ iRand;
}

//Runs all producers and consumers
void cutAll() {
    for (int iID = 0; iID < iProdCount; iID++) {
		pthread_create(&ptProducers[iID], NULL, producer, (void*) &iaIDs[iID]);
    }
    for (int iID = 0; iID < iConsCount; iID++) {
		pthread_create(&ptConsumers[iID], NULL, consumer, (void*) &iaIDs[iID]);
    }
}

//Waits for all threads to finish
void joinAll() {
    for (int i = 0; i < iProdCount; i++) {
        pthread_join(ptProducers[i], NULL);
    }
    for (int i = 0; i < iConsCount; i++) {
        pthread_join(ptConsumers[i], NULL);
    }
}
