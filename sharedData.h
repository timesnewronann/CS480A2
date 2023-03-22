/**
 * Shared constants and data structures among threads **/
#ifndef SHARED_H
#define SHARED_H

#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1
/* default number of progress marks for representing 100% progress */
#define DEFAULT_NUMOF_MARKS 50
/* minimum number of progress marks for representing 100% progress */
#define MIN_NUMOF_MARKS 10
/* place hash marks in the progress bar every N marks */
#define DEFAULT_HASHMARKINTERVAL 10
/* default minimum number of dictionary words starting from a prefix for printing or writing to the output */
#define DEFAULT_MINNUM_OFWORDS_WITHAPREFIX 1

#include <pthread.h>
#include "dictionarytree.h"
#include <iostream>
#include <queue>
using namespace std;

// Shared constants
const int NORMALEXIT = 0;
const int BADAARG_DICTIONARYSRCFILE = 1;
const int BADARG_TESTFILE = 2;
const int BADNUMOFHYPENFLAG = 3;
const int BADNUMOFHASHMARKINTERVALFLAG = 4;
const int BADNUMOFWORDSFLAG = 5;
const int BADFLAG = 10;
const int BADSYSCALL = 20;
const int THREADERROR = 30;

/* Common data shared between threads */
typedef struct
{
    /**
     * root node of the dictionary tree
     */
    trieNode *trieRootNode;

    /**
     * parameters for printing progress bar
     */
    int numOfProgressMarks;
    int hashmarkInterval;
    int minNumOfWordsWithAPrefixForPrinting;

    const char *filePath[NUMOFFILES];

    long totalNumOfCharsInFile[NUMOFFILES];

    long numOfCharsReadFromFile[NUMOFFILES];

    long wordCountInFile[NUMOFFILES];

    long numOfProcessedPrefixes;

    queue<string> prefixQueue;

    pthread_mutex_t queue_mutex;

    bool taskCompleted[NUMOFFILES];
} SHARED_DATA;

#endif