// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// node *first = NULL;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

long calc_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int word_length = strlen(word);
    char lowercase_word[word_length + 1];

    for (int i = 0; i < word_length + 1; i++)
    {
        lowercase_word[i] = tolower(word[i]);
    }
    // hash the input word
    int hashcheck = hash(lowercase_word);

    // set the head and set the cursor on the head
    node *head = table[hashcheck];
    node *cursor = head;

    if (head != NULL)
    {
        // iterate through the list comparing the strings
        while (cursor != NULL)
        {
            if (strcasecmp(lowercase_word, cursor->word) == 0)
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = (((hash << 5) + hash) + c) % N;  /* hash * 33 + c */
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // create a buffer to put your word into
    char buffer[46];

    // open file to read from and check it it was opened
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
        printf("File can't be opened\n");
    }

    //start looping through while it's not the end of the file
    while (fscanf(dict, "%s", buffer) != EOF)
    {

        //allocate memory for node element
        node *n = malloc(sizeof(node));

        //check if the memory was allocated, if not, return false
        if (n == NULL)
        {
            printf("Not enough memory.\n");
            return false;
        }
        // copy string to the buffer
        strcpy(n->word, buffer);

        int hashf = hash(buffer);

        // if the backet is empty insert the first object
        if (table[hashf] == NULL)
        {
            table[hashf] = n;
            n->next = NULL;
        }
        else
        {
            // set the pointer of n to whatever the previous node is pointing to
            n->next = table[hashf];

            // set the previous pointer to point to n
            table[hashf] = n;
        }
        calc_words++;
    }

    fclose(dict);
    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return calc_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // iterate through all the N baskets
    for (int i = 0; i < N; i++)
    {
        // set the head node
        node *head = table[i];
        node *cursor = head;


        // do until reaching the end of the linked list and free the memory you allocated before
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}