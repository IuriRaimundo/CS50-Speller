// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17576;
// Number of words in the dictionary
int dict_word_count = 0;
bool is_dict_loaded = false;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);

    // Traverse the linked-list and return true if the word is found
    node *cursor = table[index];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number, using the first three characters
unsigned int hash(const char *word)
{
    /*
        There are 677 buckets for each letter of the alphabet as the first character. (17576 / 26)
        In those 677 buckets, there are 26 buckets for each letters of the alphabet as the second character.
        And lastly, for each second letter there is only 1 bucket for each of the letters.

        Multiplying a letter's alphabet position ranging from 0-25 with the values mentioned above according to
        the letter's position in the word gives us the word's hash table position.

        If the one of the first 3 characters is a apostrophe, we treat it as a Z because it is the least used letter in the alphabet.

        Ex: word 'aaa'
            hash = 676 * 0 + 26 * 0 + 1 * 0 = 0
            word 'baa'
            hash = 676 * 1 + 26 * 0 + 1 * 0 = 677
    */
    unsigned int hash_value = 0;
    int buckets_per_character[3] = {676, 26, 1};

    for (int i = 0, n = strlen(word); i < 3 && i < n; i++)
    {

        if (isalpha(word[i]))
        {
            hash_value += buckets_per_character[i] * (tolower(word[i]) - 'a');
        }
        else
        {
            hash_value += buckets_per_character[i] * 25;
            break; // Stop the for loop if an apostrophe is found.
        }
    }
    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    // Read the file and store the words on a hash table
    char word[LENGTH];

    while (fscanf(file, "%s", word) != EOF)
    {
        // Store the word on a new node and add it to the hash table
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        unsigned int index = hash(word);
        strcpy(n->word, word);
        // Set next to NULL if the linked list is empty
        n->next = table[index] == NULL ? NULL : table[index];
        table[index] = n;
        // Increment the word counter
        dict_word_count++;
    }
    fclose(file);
    is_dict_loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (is_dict_loaded)
    {
        return dict_word_count;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Traverse the linked-list and free every node
    for (int i = 0; i < N - 1; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
