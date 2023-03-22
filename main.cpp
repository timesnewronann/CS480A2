#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include "dictionarytree.h"
#include <sys/stat.h>
#include <sys/types.h>

#include "sharedData.h"

// NOTE this source code file is for simple demonstration
// of how command line parsing and pthreads work,
// this file MAY NOT compile

// Thread for populating a dictionary tree
#include "populatetree.h"
// Thread for reading in prefixes and queuing them
#include "readprefix.h"
// Thread for searching and counting prefixes in a dictionary tree
#include "countprefix.h"

#define NULL_INTEGER_VALUE 0
#define BADFLAG 1
#define INSUFFICIENT_PROG_MARK_FLAG 1
#define OUT_OF_RANGE_HASHTAG 2
#define FILE_NOT_FOUND_VALUE 3

using namespace std; // Make sure std components accessible

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */

int main(int argc, char **argv)
{
    // bool verbose = false; // chatty or quiet program
    int hash_mark;     // hash mark for progress bar
    int progress_mark; // progress mark for displaying progress
    int option;        /* command line switch */
    int count;         // some counter set by -n, set default
                       // other stuff (e.g. declarations)

    bool taskCompleted[NUMOFFILES]; // boolean to check if the task was finished
    SHARED_DATA shared;             // shared data object

    // shared.filePath[0] = "vocabFile.txt"; // text file

    // REMOVE MAGIC NUMBERS
    // PROPER ERROR HANDLING WITH FILE NOT FOUND
    // CLEAN UP ADD COMMENTS
    // HAVE IT EXACTLY LIKE DOC

    // create the shared object file paths
    shared.totalNumOfCharsInFile[DICTSRCFILEINDEX] = 0;  // initialize the total number of characters in the file
    shared.totalNumOfCharsInFile[TESTFILEINDEX] = 0;     // initialize the total number of characters in the file
    shared.numOfCharsReadFromFile[DICTSRCFILEINDEX] = 0; // initialize the number of characters read from the file
    shared.numOfCharsReadFromFile[TESTFILEINDEX] = 0;    // initialize the number of characters read from the file

    shared.wordCountInFile[DICTSRCFILEINDEX] = 0; // initialize the number of words in the file
    shared.wordCountInFile[TESTFILEINDEX] = 0;    // initialize the number of words in the file

    shared.numOfProcessedPrefixes = 0;              // initialize the number of processed prefixes
    shared.taskCompleted[DICTSRCFILEINDEX] = false; // initialize the task completed
    shared.taskCompleted[TESTFILEINDEX] = false;    // initialize the task completed

    // the default values of the arguments
    // shared.numOfProgressMarks = DEFAULT_NUMOF_MARKS;
    // shared.hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
    shared.numOfProcessedPrefixes = 0; // initialize the number of processed prefixes
    // shared.minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;

    int n = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX; // default value
    int p = DEFAULT_NUMOF_MARKS;                // default value
    int h = DEFAULT_HASHMARKINTERVAL;           // default value

    // used for passing in the arguments
    while ((option = getopt(argc - 2, &argv[2], "p:h:n:")) != -1)
    {
        // switch case for the arguments
        switch (option)
        {
        case 'h':
            h = atoi(optarg);
            if (h <= 0 or h > 10)
            {
                cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
                exit(OUT_OF_RANGE_HASHTAG); // error handling
            }
            break;

        case 'n':
            n = atoi(optarg);
            break;
        case 'p':
            p = atoi(optarg);
            if (p < 10)
            {
                cout << "Number of progress marks must be a number and at least 10" << endl;
                exit(INSUFFICIENT_PROG_MARK_FLAG); // error handling
            }
            break;

        default:
            exit(BADFLAG); // BADFLAG is an error # defined in a header
        }
    }

    shared.numOfProgressMarks = p;                  // set the number of progress marks
    shared.hashmarkInterval = h;                    // set the hash mark interval
    shared.minNumOfWordsWithAPrefixForPrinting = n; // set the minimum number of words with a prefix for printing

    // possible error checking
    // error checking wasn't working

    pthread_mutex_init(&shared.queue_mutex, NULL); // initialize the mutex
    shared.filePath[DICTSRCFILEINDEX] = argv[1];   // set the file path
    shared.filePath[TESTFILEINDEX] = argv[2];      // set the file path

    shared.trieRootNode = new trieNode(); // create the root node

    // Threads
    pthread_t populateTreeThread;
    pthread_t readPrefixThread;
    pthread_t countPrefixThread;

    // Spawn pop tree
    if (pthread_create(&populateTreeThread, NULL, &populateTree, &shared))
    {
        // error handling
        cout << "Error occured in populateTree" << endl;
        exit(1);
    }

    const char *file1 = shared.filePath[DICTSRCFILEINDEX]; // get the file path
    const char *file2 = shared.filePath[TESTFILEINDEX];    // get the file path
    struct stat sb;                                        // stat buffer

    // stat functions to calculate the progress bar
    if (stat(file1, &sb) == 0)
    {
        shared.totalNumOfCharsInFile[DICTSRCFILEINDEX] = sb.st_size / sizeof(char); // get the total number of characters in the file
    }

    if (stat(file2, &sb) == 0)
    {
        shared.totalNumOfCharsInFile[TESTFILEINDEX] = sb.st_size / sizeof(char); // get the total number of characters in the file
    }

    int currMarks = 0; // variable for the current amounnt of marks created
    // cout << "Before while Loop" << endl;
    while (shared.numOfCharsReadFromFile[DICTSRCFILEINDEX] != shared.totalNumOfCharsInFile[DICTSRCFILEINDEX])
    {
        // cout << "Char Read: " << shared.numOfCharsReadFromFile[0] << "\nChar Total" << shared.totalNumOfCharsInFile[0] << endl;
        int predictedMarks = ((double)shared.numOfCharsReadFromFile[DICTSRCFILEINDEX] / (double)shared.totalNumOfCharsInFile[DICTSRCFILEINDEX]) * shared.numOfProgressMarks; // calculate the number of progress marks

        while (currMarks < predictedMarks + 1) // while the current number of marks is less than the predicted number of marks
        {
            if ((currMarks + 1) % shared.hashmarkInterval == 0) // if the current number of marks is a multiple of the hash mark interval
            {

                cout << "#" << flush; // print a hash mark
                currMarks++;          // increment the current number of marks
            }
            else
            {
                cout << "-" << flush; // print a dash
                currMarks++;          // increment the current number of marks
            }
            cout.flush();
        }
    }
    cout << "\nThere are " << shared.wordCountInFile[DICTSRCFILEINDEX] << " words in vocabulary.txt." << endl; // print the number of words in the file

    currMarks = 0; // reset the current number of marks

    // busy waiting
    while (!shared.taskCompleted[DICTSRCFILEINDEX])
    {
        continue;
    }

    // Spawn both read and count
    if (pthread_create(&readPrefixThread, NULL, &readPrefix, &shared))
    {
        // error handling
        cout << "Error occured in readPrefix" << endl;
        exit(1);
    }

    // spawn in count
    if (pthread_create(&countPrefixThread, NULL, &countprefix, &shared))
    {
        cout << "Error occured in countPrefix" << endl;
        exit(1);
    }
    // busy waiting for read
    while (!shared.taskCompleted[TESTFILEINDEX])
    {
        continue;
    }

    // Progress Bar for Testfile index needs to be made after read and count threads
    while (shared.numOfProcessedPrefixes != shared.wordCountInFile[TESTFILEINDEX])
    {
        int predictedMarks = ((double)shared.numOfProcessedPrefixes / (double)shared.wordCountInFile[TESTFILEINDEX]) * shared.numOfProgressMarks; // calculate the number of progress marks

        while (currMarks < predictedMarks + 1)
        {
            if ((currMarks + 1) % shared.hashmarkInterval == 0) // if the current number of marks is a multiple of the hash mark interval
            {
                cout << "#"; // create hashes
                currMarks++;
            }
            else
            {
                cout << "-"; // create dashes
                currMarks++;
            }
            cout.flush(); // instant output
        }
    }
    cout << "\nThere are " << shared.wordCountInFile[1] << " words in testfile1.txt." << endl; // print the number of words in the file

    pthread_mutex_destroy(&shared.queue_mutex); // destroy the mutex
}