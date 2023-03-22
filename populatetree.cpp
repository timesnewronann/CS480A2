#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include "sharedData.h"
using namespace std;

// Read words from a text file and populate the dictionary tree
void *populateTree(void *voidPtr)
{
    // create an instance of the node
    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    // read similar to the readPrefix
    //  used for tokenizing
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";

    trieNode root;
    shared->trieRootNode = &root;

    // busy wait for the dictionary trees to be populated
    //
    // read the file that contains prefixes
    ifstream textstream(shared->filePath[DICTSRCFILEINDEX]);
    string line;

    //

    // iterate over file line by line
    while (getline(textstream, line))
    {
        // grab the first token, strtok is not safe use strtok_r
        char *word = strtok(const_cast<char *>(line.c_str()), delimiters);
        shared->wordCountInFile[DICTSRCFILEINDEX]++; // increment the word
        // cout << shared->numOfCharsReadFromFile[0] << endl; // correct number
        shared->numOfCharsReadFromFile[DICTSRCFILEINDEX] += line.length() + 1; // increment the token
        // cout << shared->numOfCharsReadFromFile[DICTSRCFILEINDEX] << endl;
        //  tokenizing each string
        if (word != nullptr)
        {
            shared->trieRootNode->insert(word);
        }
    }

    // set task completion for reading and queuing prefix
    //......
    // change the task Completed variable
    // taskCompleted[NUMOFFILES];
    // maybe set this in main for the progress bar
    shared->taskCompleted[DICTSRCFILEINDEX] = true;

    return nullptr;
}
