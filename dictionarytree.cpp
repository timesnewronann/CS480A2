#include <iostream>
#include "dictionarytree.h"
#include <cstring>
using namespace std;
// RONAN BORJA 825164880
//  don't need to call trieNode already because its in the header file

// helper function that maps all of the next[nchild] to null
// hard codes function
int trieNode::mapper(const char letter)
{
    int childIndex;
    // hard code
    if (letter == 'a' || letter == 'A')
    {
        childIndex = 0;
    }

    else if (letter == 'b' || letter == 'B')
    {
        childIndex = 1;
    }

    else if (letter == 'c' || letter == 'C')
    {
        childIndex = 2;
    }

    else if (letter == 'd' || letter == 'D')
    {
        childIndex = 3;
    }

    else if (letter == 'e' || letter == 'E')
    {
        childIndex = 4;
    }
    else if (letter == 'f' || letter == 'F')
    {
        childIndex = 5;
    }
    else if (letter == 'g' || letter == 'G')
    {
        childIndex = 6;
    }
    else if (letter == 'h' || letter == 'H')
    {
        childIndex = 7;
    }

    else if (letter == 'i' || letter == 'I')
    {
        childIndex = 8;
    }

    else if (letter == 'j' || letter == 'J')
    {
        childIndex = 9;
    }
    else if (letter == 'k' || letter == 'K')
    {
        childIndex = 10;
    }
    else if (letter == 'l' || letter == 'L')
    {
        childIndex = 11;
    }
    else if (letter == 'm' || letter == 'M')
    {
        childIndex = 12;
    }
    else if (letter == 'n' || letter == 'N')
    {
        childIndex = 13;
    }
    else if (letter == 'o' || letter == 'O')
    {
        childIndex = 14;
    }
    else if (letter == 'p' || letter == 'P')
    {
        childIndex = 15;
    }
    else if (letter == 'q' || letter == 'Q')
    {
        childIndex = 16;
    }
    else if (letter == 'r' || letter == 'R')
    {
        childIndex = 17;
    }
    else if (letter == 's' || letter == 'S')
    {
        childIndex = 18;
    }
    else if (letter == 't' || letter == 'T')
    {
        childIndex = 19;
    }
    else if (letter == 'u' || letter == 'U')
    {
        childIndex = 20;
    }
    else if (letter == 'v' || letter == 'V')
    {
        childIndex = 21;
    }
    else if (letter == 'w' || letter == 'W')
    {
        childIndex = 22;
    }
    else if (letter == 'x' || letter == 'X')
    {
        childIndex = 23;
    }
    else if (letter == 'y' || letter == 'Y')
    {
        childIndex = 24;
    }
    else if (letter == 'z' || letter == 'Z')
    {
        childIndex = 25;
    }
    else if (letter == '\'')
    {
        childIndex = 26;
    }
    else if (letter == '-')
    {
        childIndex = 27;
    }
    else if (letter == '_')
    {
        childIndex = 28;
    }
    else
    {
        childIndex = 29;
    }
    return childIndex;
}

// helper function that converts characters to integers

// check if node was properly inserted
bool trieNode ::insert(const char *wordBeingInserted)
{

    // create an instance of the node
    trieNode *node = this;

    // wordBeingInserted is already null
    for (int i = 0; i < strlen(wordBeingInserted); i++)
    {
        // check if the word inserted is a current letter
        int letterIndex = mapper(wordBeingInserted[i]);

        // next[0] = new node
        //  you know have an index

        // if the index is null then make a new node
        if (node->next[letterIndex] == nullptr)
        {
            node->next[letterIndex] = new trieNode();
        }
        node = node->next[letterIndex];

        // if the index is equal to the terminator
        // check if the child is in the terminator
        // if (node->mapper('\0')) never hits the if statement
        // {
        //     node->next[letterIndex] = new trieNode();
        //     return true;
        // }
    }
    node->next[29] = new trieNode();

    // return false not inserted in yet
    return true;
}

/*
bool trieNode :: insert(**root, char *signedtext)
{
    if(*root == NULL)
    {
        *root = createNode();

    }
    unsigned char *text = (unsigned char*) signedtext;
    trienode *tmp = *root;
    int length = strlen(signedtext);

    for(int i = 0; i < length; i++)
    {
        if(tmp -> children[text[i]] == NULL)
        {
            //create new node
            tmp ->children[text[i]] = createnode();
        }
        tmp = tmp -> children[text[i]];
    }
    if(tmp-> terminal)
    {
        return False;
    }
    else
    {
        tmp-> terminal = true;
        return true;
    }
}
 */

// count the words from the prefix searching
//  initially called on a starting node to start the counting
//  Starting from the dictNode, use recursive calls (or iterative loop)
//  to traverse the whole subtree from the node(including the node) to
// count all words that start from the node
//  Use the count argument passed by reference to count

void trieNode ::countWordsStartingFromANode(int &count)
{
    // recursive
    //  check if the array has reached the terminator
    if (this->next[29] != nullptr)
    {
        // increase the count because the word has finished
        count++;
    }

    // go through each letter and check if it exists
    for (int i = 0; i < 29; i++)
    {
        if (this->next[i] != nullptr)
        {
            // the letter exists
            this->next[i]->countWordsStartingFromANode(count);
        }
    }
}

/* For searching the tree to count the words that start with a specific string or prefix:
    • First call findEndingNodeOfAStr to find node that ends the string or
      prefix,
    • Then call countWordsStartingFromANode to count the words starting
      from the node returned from findEndingNodeOfAStr  */
// search within the trie
// searching start from the root go to the first letter
// change pointer to the next letter
// change pointer if it exists
/// if it doesn't exist return null
// keep doing it until you get the last letter
// return the node of word of you're searching
trieNode *trieNode::findEndingNodeOfAStr(const char *strBeingSearched)
{
    // This is really similar to insert
    // create an instance of the node
    trieNode *currentNode = this;

    // loop through the indices of the str
    for (int i = 0; i < strlen(strBeingSearched); i++)
    {
        // convert the str into numbers
        int letterIndex = mapper(strBeingSearched[i]);
        if (currentNode->next[letterIndex] == nullptr)
        {
            // return the node
            return new trieNode();
        }
        currentNode = currentNode->next[letterIndex];
    }
    // return the node
    return currentNode;

    // convert the strBeingSearched into an integer
}
