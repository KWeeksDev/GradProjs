#define DICTIONARY_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "Boggle.h"
#include "Node.h"
#include "Dictionary.h"
#include "game.h"

node* createNode(char value)
{
	node* new_Node = new node;
	new_Node -> val = value;
	for(int i=0;i<NUMBER_OF_LETTERS_IN_ALPHABET;i++)
	{
		new_Node -> next_Letter[i] = 0x00;
	}
	new_Node -> is_Word = false;
	new_Node -> is_Last_Letter = false;
	return new_Node;
}
void parseDictionaryFile(char8_t *filename, int32_t *numWords)
{	
	char temp = 0;
	char word_Length = 0;
	FILE* dictionaryFile = NULL;

	dictionaryFile = fopen(filename,"r");
	if(dictionaryFile == NULL)
	{
		printf("Error opening file %s \n", filename);
	}
	

	dictionary_Struct = createNode('\n');
	node * pointer = dictionary_Struct;
	while(!feof(dictionaryFile))
	{
		fscanf(dictionaryFile,"%c",&temp);
		if((temp >= 'A') && (temp <='Z'))
		{
			if(pointer -> next_Letter[temp-'A'] == 0)
			{
				pointer -> next_Letter[temp - 'A'] = createNode(temp);
			}
			pointer = pointer -> next_Letter[temp - 'A'];
			pointer -> is_Last_Letter = false;
			word_Length++;
			
		}
		else if ((pointer != dictionary_Struct) && (temp == '\n'))
		{
			if(word_Length > MINIMUM_WORD_LENGTH)
			{
				pointer -> is_Word = true;
				pointer -> is_Last_Letter = true;
			}	
			word_Length = 0;
			pointer = dictionary_Struct;
		}
	}
	pointer = dictionary_Struct;
	fclose(dictionaryFile);
}


