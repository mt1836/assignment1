#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
  char str[LENGTH];
  char str2[LENGTH];
  char str3[LENGTH];

  strncpy(str,word,LENGTH);
  strcpy(str2,lower_case(str));
  strcpy(str3,remove_punctuation(str2));
  bucket=hash_function(str3);
  cursor=hashtable[bucket];
  while(cursor!=NULL)
  {
    if(strcmp(str3,cursor->word)==0)
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
  char *word;
  char str[LENGTH];
  char str2[LENGTH];
  char *str3;

  word=(char*)malloc((LENGTH)*sizeof(char));

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
          break;
        }
        else
        {
          word[counter]='\0';
          counter=0;
          if(!check_word(word,hashtable))
          {
            str3=(char*)malloc((LENGTH)*sizeof(char));
            strncpy(str,word,LENGTH);
            //commment
    //        printf("str at FEOF=%s\n",word);
            free(word);
            strcpy(str2,lower_case(str));
            strcpy(str3,remove_punctuation(str2));
            misspelled[num_misspelled]=str3;
            num_misspelled++;
            break;
          }
          else
          {
            free(word);
            break;
          }
        }
      }
      if(!isspace(c))
      {
        if(counter>LENGTH)
        {
          continue;
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
          continue;
        }
        else
        {
          word[counter]='\0';
          counter=0;
          if(!check_word(word,hashtable))
          {
            str3=(char*)malloc((LENGTH)*sizeof(char));
            strncpy(str,word,LENGTH);
  //          printf("str at not=%s\n",word);
            free(word);
            strcpy(str2,lower_case(str));
            strcpy(str3,remove_punctuation(str2));
            misspelled[num_misspelled]=str3;
            num_misspelled++;
          }
          else
          {
            free(word);
          }
          word=(char*)malloc((LENGTH+1)*sizeof(char));
        }
      }
    } while(1);
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

char* remove_punctuation(char* str)
{
  int index,jindex;

  for(int i=0;i<strlen(str);i++)
  {
    if(isalpha(str[i]))
    {
      index=i;
      break;
    }
  }
  for(int j=strlen(str)-1; j>=0;j--)
  {
    if(isalpha(str[j]))
    {
      jindex=j;
      break;
    }
  }
  for(int a=index;a<=jindex;a++)
  {
    str[a-index]=str[a];
  }
  for(int b=(jindex+1)-index; str[b]!='\0';b++)
  {
    str[b]='\0';
  }
  return str;
}
