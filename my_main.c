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
char* misspelled[MAX_MISSPELLED];

int main(int argc, char *argv[])
{
  load_dictionary(argv[2], hash);
//  check_words(fopen(argv[1],"r"), hash, misspelled);
  printf("num_misspelled=%d\n",check_words(fopen(argv[1],"r"), hash, misspelled));
  return 0;
}
