#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <check.h>
#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]);
bool check_word(const char* word, hashmap_t hashtable[]);
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]);

hashmap_t hash[HASH_SIZE];
hashmap_t temp;
char* misspelled[MAX_MISSPELLED];


int main(int argc, char *argv[])
{
  FILE *fp;
  fp = fopen(argv[1], "r");
  load_dictionary(argv[2], hash);
//  check_words(fopen(argv[1],"r"), hash, misspelled);
  printf("num_misspelled=%d\n",check_words(fp, hash, misspelled));
  for(int f=0;f<HASH_SIZE;f++)
  {
    while(hash[f])
      {
        temp=hash[f]->next;
        free(hash[f]);
        hash[f]=temp;
      }
  }
  for(int m=0;m<MAX_MISSPELLED;m++)
  {
    printf("misspelled[%d]=%s\n",m,misspelled[m]);
    free(misspelled[m]);
  }
  fclose(fp);
  return 0;
}
