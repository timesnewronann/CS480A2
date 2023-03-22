#include <iostream>
#include "sharedData.h"
#include <fstream>
#include <unordered_map>

// read prefixes from the text file and insert them into
// a prefix queue

/**
 * void * readPrefixToQueue(void *)
 * Threadentry point. Argument must be a pointer to a SHARED_DATA structure
 * Which contains the communication channel and thread parameter
 *
 * readPrefixToQueue thread is responsible for reading words/prefix from a file
 * name passed from the shared buffer); and putting each prefix in a queue
 * */
void *readPrefix(void *voidPtr)
{
    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    if (shared == NULL)
        return NULL;

    // used for tokenizing
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";

    ifstream textstream(shared->filePath[TESTFILEINDEX]);
    string line;
    // iterate over file line by line

    // busy waiting
    while (!shared->taskCompleted[DICTSRCFILEINDEX])
    {
        continue;
    }
    

    while (getline(textstream, line))
    {
        // grab the first token, strtok is not safe use strtok_r
        char *token = strtok(const_cast<char *>(line.c_str()), delimiters);
        while (token != nullptr)
        {
            // mutex lock
            pthread_mutex_lock(&(shared->queue_mutex));

            shared->prefixQueue.push(token);

            // mutex unlock
            pthread_mutex_unlock(&(shared->queue_mutex));

            // update the number of words read
            shared->wordCountInFile[TESTFILEINDEX]++;

            token = strtok(NULL, delimiters);
        }
    }
    shared->taskCompleted[TESTFILEINDEX] = true; // set task completion for reading and queuing prefix

    return nullptr;
}