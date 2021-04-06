// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table - apostrophes can't be word-initial, and as the buckets are linked lists rather than tries, they only represent the possible first letters of words (the 26 letters of the alphabet)
const unsigned int N = 26;

// Hash table
node *table[N];

// variable for storing word count (to be iterated in load and referenced in size)
unsigned int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // hash word
    unsigned int index = hash(word);

    // set check node to the node at the resulting index in the table
    node *node_to_check = table[index];

    // if the node is not null, check if its word if the word being checked, else move on to the next node if one exists
    while (node_to_check != NULL)
    {
        // return true if the words match
        if (strcasecmp(node_to_check->word, word) == 0)
        {
            return true;
        }

        // move to the next node if they don't
        node_to_check = node_to_check->next;
    }

    // return false if the word is not found in the dictionary
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // if first letter of word is upper case, subtract 65 so that 'a' is zero-indexed
    if (word[0] < 97)
    {
        return word[0] - 65;
    }
    // if first letter of word is lower case, subject 97 so that 'a' is zero-indexed
    else
    {
        return word[0] - 97;
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file; check not null
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file\n");
        return false;
    }

    // variable to store word
    char current_word[LENGTH + 1];

    // variable for storing hash return value
    unsigned int index;

    // until end of file, store word in word variable...
    while (fscanf(file, "%s", current_word) != EOF)
    {
        // allocate space for a new node
        node *current_node = malloc(sizeof(node));

        // check node/result of malloc is not null
        if (current_node == NULL)
        {
            return false;
        }

        // all being well, copy the current word into the current node
        strcpy(current_node->word, current_word);

        // get result of hashing current word
        index = hash(current_word);

        // set current node to point to the existing node in the table at the resulting index (from hashing the word)
        current_node->next = table[index];

        // set the index of the table to point to the current node
        table[index] = current_node;

        // iterate word_count
        word_count++;
    }

    // close dictionary file and return true
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // for each node
    for (int i = 0; i < N; i++)
    {
        // set the node to check as the ith index in the hash table (ith bucket)
        node *node_to_check = table[i];

        // until the node to check is null
        while (node_to_check != NULL)
        {
            // store the current non-null node in a new node to free variable
            node *node_to_free = node_to_check;

            // shift the node to check
            node_to_check = node_to_check->next;

            // free the node
            free(node_to_free);
        }

        // if the last bucket has been reached and the node to check is null (i.e. there are no more nodes to free), return true
        if (i == N - 1 && node_to_check == NULL)
        {
            return true;
        }
    }

    return false;
}