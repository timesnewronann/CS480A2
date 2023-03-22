#include <iostream>
#include <fstream>
#include "dictionarytree.h"
#include <cstring>
#include "string.h"
#include <pthread.h>
#include "sharedData.h"
#include "dictionarytree.h"
using namespace std;

// RONAN BORJA 825164880
/*
a. search and count the number of words in the dictionary tree that start with the
prefix,
b. if the count is equal to or greater than the number specified by an optional
command line argument (default is 1 if not specified, see below), print this
prefix and the count (separated by a space) to a file named
countprefix_output.txt, ONE line per prefix, in the exact format as:
hour 10
‘hour 10’ here means for the ‘hour’ read from test file, there are 10 words in the
dictionary tree that start with ‘hour
 **/

// CountPrefix, I will use the shared data queue and pop the words read in from readPrefix

// think about the while loop in tasks done and queue size

// DO this in while loop() : What is the condition?
// After popping out words in the queue count the words and check the prefix
// if count >= commandLine arg:
// print the prefix and count into
// countprefix_output.txt
// hour (word) 10 (10 words)
// update number of processed prefixes

// search ending word of string and count

// after grab each prefix and search them from in dictionary tree
// fill in the text file
// as I read in the prefix and removing the prefix from the queue
// remove one word from the queue and go to dictionary queue and count it

void *countprefix(void *voidPtr)
{
    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    if (shared == NULL)
        return NULL;

    ofstream file; // create the file

    file.open("countprefix_output.txt"); // open the output file

    // busy waiting
    while (!shared->taskCompleted[DICTSRCFILEINDEX])
    {
        continue;
    }

    // while condition to count the words popped out

    while (shared->taskCompleted[1] == false || !shared->prefixQueue.empty())
    {
        if (!shared->prefixQueue.empty())
        {
            string word;
            // Lock mutex
            pthread_mutex_lock(&(shared->queue_mutex));

            // word = queue.front()
            word = shared->prefixQueue.front();

            // pop the queue
            shared->prefixQueue.pop();

            // unlock the mutex
            pthread_mutex_unlock(&(shared->queue_mutex));

            // init count = 0
            int count = 0;

            // call findNodeStartingFromString on the word you just popped
            shared->trieRootNode->findEndingNodeOfAStr(word.c_str())->countWordsStartingFromANode(count);

            // call count function and pass in count
            // shared->trieRootNode->countWordsStartingFromANode(count);
            // check if count >= shared.MinPrinting arg

            // place into output file

            shared->numOfProcessedPrefixes++;

            // remove queue search and output
            if (count >= shared->minNumOfWordsWithAPrefixForPrinting)
            {
                // print the prefix and the count
                // cout << word << " " << count << endl;
                file << word << " " << count << endl; // print in the prefix into the file
            }
        }
    }

    file.close(); // close the file
}
