#include <stdbool.h>
#include <string.h>
#include <ctype.h>
//#include <check.h>
#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]);
bool check_word(const char* word, hashmap_t hashtable[]);
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]);
char* remove_punctuation(char* word);
char* lower_case(char* str);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
/**
 * Inputs:
 *  dictionary_file:    Path to the words file.
 *  hashtable:          The hash table to be populated.
 *
 * Returns:
 *  bool:       Whether or not the hashmap successfully populated.
 *
 * Modifies:
 *  hashtable: This hashmap should be filled with words from the file provided.
 *
 * Example:
 *  bool success = load_dictionary("wordlist.txt", hashtable);
 **/
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
  int key;
  hashmap_t new_node;

  for(int i=0;i<HASH_SIZE;i++)
  {
    hashtable[i]=NULL;
  }
  FILE *fp;
  char str[LENGTH];
  fp = fopen(dictionary_file, "r");
  if (fp == NULL)
  {
//      printf("Could not open file %s\n",dictionary_file);
//      perror("Error printed by perror");
      return 1;
  }
   while(fgets(str, LENGTH+2, fp)!= NULL)
  {
    str[strcspn(str, "\n")] = 0;
    new_node=malloc(sizeof(node));
    new_node->next = NULL;
    lower_case(str);
    strcpy(new_node->word, str);
    key = hash_function(str);
    if(hashtable[key] == NULL)
    {
      hashtable[key] = new_node;
    }
    else
    {
      new_node->next=hashtable[key];
      hashtable[key]=new_node;
    }
  }
  fclose(fp);
  return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[])
{
  int bucket;
  hashmap_t cursor;

  bucket=hash_function(word);
  cursor=hashtable[bucket];
  while(cursor!=NULL)
  {
    if(strcmp(word,cursor->word)==0)
    {
      return true;
    }
    cursor=cursor->next;
  }
  return false;
}

/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
/**
 * Inputs:
 *  fp:         A file pointer to the document to check for spelling errors.
 *  hashtable:  The hash table used to determine spelling
 *  misspelled: An empty char* array to be populated with misspelled words.
 *              This array will never be greater than 1000 words long.
 *
 * Returns:
 *  int:        The number of words in the misspelled arary.
 *
 * Modifies:
 *  misspelled: This array will be filled with misspelled words.
 *
 * Example:
 *  int num_misspelled = check_words(text_file, hashtable, misspelled);
 **/
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
  int num_misspelled=0;
  int c, counter=0;
  int n = 0;
  char *word;
  hashmap_t temp;
  word=(char*)malloc(LENGTH*sizeof(char));
   if(fp == NULL)
   {
//      perror("Error in opening file");
      return(-1);
   }
   do
   {
      c = fgetc(fp);
      if(feof(fp))
      {
        if(counter==0)
        {
          free(word);
          for(int f=0;f<HASH_SIZE;f++)
          {
            while(hashtable[f])
              {
                temp=hashtable[f]->next;
                free(hashtable[f]);
                hashtable[f]=temp;
              }
          }
          break;
        }
        else
        {
          word[counter]='\0';
          counter=0;
          lower_case(word);
          remove_punctuation(word);
          if(!check_word(word,hashtable))
          {
            misspelled[num_misspelled]=word;
            num_misspelled++;
          }
          free(word);
          for(int f=0;f<HASH_SIZE;f++)
          {
            while(hashtable[f])
              {
                temp=hashtable[f]->next;
                free(hashtable[f]);
                hashtable[f]=temp;
              }
          }
          break;
        }
      }
      if(!isspace(c))
      {
        if(counter>LENGTH-1)
        {
//          word[counter]='\0';
//          counter=0;
//          break;
        }
        else
        {
          word[counter]=c;
          counter++;
        }
      }
      if(isspace(c))
      {
        if(counter==0)
        {
          free(word);
          for(int f=0;f<HASH_SIZE;f++)
          {
            while(hashtable[f])
              {
                temp=hashtable[f]->next;
                free(hashtable[f]);
                hashtable[f]=temp;
              }
          }
          break;
        }
        else
        {
          word[counter]='\0';
          counter=0;
          lower_case(word);
          remove_punctuation(word);
          if(!check_word(word,hashtable))
          {
            misspelled[num_misspelled]=word;
            num_misspelled++;
          }
//          free(word);
          word=(char*)malloc(LENGTH*sizeof(char));
        }
      }
    } while(1);
//    fclose(fp);
   return num_misspelled;
 }

char* lower_case(char* str)
{
  for (int i = 0; str[i]!='\0'; i++)
  {
    if(str[i] >= 'A' && str[i] <= 'Z')
    {
      str[i] = str[i] + 32;
    }
  }
  return str;
}

char* remove_punctuation(char* word)
{
  int index,jindex,b;

  for(int i=0;i<strlen(word);i++)
  {
    if(isalpha(word[i]))
    {
      index=i;
      break;
    }
  }
  for(int j=strlen(word)-1; j>=0;j--)
  {
    if(isalpha(word[j]))
    {
      jindex=j;
      break;
    }
  }
  for(int a=index;a<=jindex;a++)
  {
    word[a-index]=word[a];
  }
  for(int b=(jindex+1)-index; word[b]!='\0';b++)
  {
    word[b]='\0';
  }
  return word;
}
