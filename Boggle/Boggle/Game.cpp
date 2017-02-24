
#define GAME_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "types.h"
#include "Boggle.h"
#include "Node.h"
#include "Dictionary.h"
#include "dice.h"
#include "game.h"

//Arrays of defined dice and game square structs
#if BIG_BOGGLE
char* diceUsed[NUM_DICE] = {DIE1, DIE2, DIE3, DIE4, DIE5, DIE6, DIE7, DIE8, DIE9, DIE10, DIE11, DIE12, DIE13, DIE14, DIE15, DIE16, DIE17, DIE18, DIE19, DIE20, DIE21, DIE22, DIE23, DIE24, DIE25};
grid_Neighbor * game_Board_Squares[NUM_DICE] = {square1,square2,square3,square4,square5,square6,square7,square8,square9,square10,square11,square12,square13,square14,square15,square16,square17,square18,square19,square20,square21,square22,square23,square24,square25};
#else
char* diceUsed[NUM_DICE] = {DIE1, DIE2, DIE3, DIE4, DIE5, DIE6, DIE7, DIE8, DIE9, DIE10, DIE11, DIE12, DIE13, DIE14, DIE15, DIE16};
grid_Neighbor * game_Board_Squares[NUM_DICE] = {square1,square2,square3,square4,square5,square6,square7,square8,square9,square10,square11,square12,square13,square14,square15,square16};
#endif

//Varaibles 
char  word_length_Counter = 0;
char  dice_Value = 0;
char  dice_Picked = 0;
char  dice_Remaining = 0;
char* words_Found = NULL; 
char* words_Found_Pointer = NULL;
char* game_Grid = NULL; 
char* game_Grid_Pointer = NULL;
char* possible_Word = NULL; 
char* possible_Word_Pointer = NULL;
wordListT* word_List_Pointer = NULL;
wordListT* word_List_Pointer_Two = NULL;
grid_Neighbor * grid_Pointer = NULL;

//Initializes a game grid square
grid_Neighbor* createBlankGrid(char game_Dice)
{
	grid_Neighbor * new_Neighbor = new grid_Neighbor;
	new_Neighbor -> letter = game_Dice;

	for(int i = 0; i < MAX_POSSIBLE_GRID_NEIGHBORS; i++)
	{
		new_Neighbor -> neighbor[i] = 0;
	}
	new_Neighbor -> visited = false;
	return new_Neighbor;
}

// Adds word to wordList struct
wordListT* pushWordList(char* word)
{
	wordListT* newWord = new wordListT;
	newWord -> word = word;
	newWord -> nextWord = 0;
	return newWord;
}

//Frees malloc memory and  resets varaibles
void resetGame() 
{
	free(words_Found);
	free(game_Grid);
	free(possible_Word);

	words_Found_Pointer = words_Found;
	game_Grid_Pointer = game_Grid;
	possible_Word_Pointer = possible_Word;
	topOfWordList = 0x00;
	word_List_Pointer = topOfWordList;
	word_List_Pointer_Two = topOfWordList;
	grid_Pointer = head_Of_Grid;
}

//initializes all setup needs
void initGame()	
{
	words_Found = (char*)malloc(1000*(sizeof(char)*33));
	game_Grid = (char*)malloc((NUM_DICE+1)*sizeof(char));
	possible_Word = (char*)malloc((MAX_CHARS_IN_DICTIONARY_WORD+1)*sizeof(char));


	word_length_Counter = 0;
	words_Found_Pointer = words_Found;
	game_Grid_Pointer = game_Grid;
	possible_Word_Pointer = possible_Word;
	topOfWordList = pushWordList("");
	word_List_Pointer = topOfWordList;
	word_List_Pointer_Two = topOfWordList;
	grid_Pointer = head_Of_Grid;
	dice_Remaining = NUM_DICE;
}	

//nuilds random dice board based on chosed board size	
void buildRandomBoard() 
{
	char* temp_String = "";
	//Randomize first number a bit
	srand((unsigned int)time(NULL));
	dice_Value = rand() % 6;
	dice_Value = rand() % 6;
	//For each dice provided
	for(int i =0; i < NUM_DICE; i++)
	{
		dice_Value = rand() % 6;
		if(dice_Remaining > 0)
		{
			dice_Picked = rand() % dice_Remaining;
			*game_Grid_Pointer = diceUsed[dice_Picked][dice_Value];
			game_Grid_Pointer++;
			temp_String = diceUsed[dice_Picked];
			diceUsed[dice_Picked] = diceUsed[dice_Remaining-1];
			diceUsed[dice_Remaining-1] = temp_String;
			dice_Remaining--;
		}
	}
	game_Grid_Pointer = game_Grid;

	//Create an square for each dice that will hold pointers to all potential neighbors
	for(int i = 0; i < NUM_DICE; i++)
	{
		game_Board_Squares[i] = createBlankGrid(*game_Grid_Pointer);
		game_Grid_Pointer++;
	}
	game_Grid_Pointer = game_Grid;

	//For each grid slot, check and assign appropriate neighboring grid locations
	for(int current_Square = 0; current_Square < NUM_DICE; current_Square++)
	{
			//Upper Left Neighbor
			if((current_Square / NUM_ROWS != 0) && (current_Square % NUM_COLS != 0))
			{
				game_Board_Squares[current_Square] -> neighbor[0] = game_Board_Squares[((current_Square - NUM_ROWS)-1)];
			}
			//Upper Neighbor
			if(current_Square / NUM_ROWS != 0)
			{
				game_Board_Squares[current_Square] -> neighbor[1] = game_Board_Squares[(current_Square - NUM_ROWS)];
			}
			//Upper Right Neighbor
			if((current_Square / NUM_ROWS != 0) && (current_Square % NUM_COLS != NUM_COLS-1))
			{
				game_Board_Squares[current_Square] -> neighbor[2] = game_Board_Squares[(current_Square - NUM_ROWS + 1)];
			}
			//Left Neighbor
			if(current_Square % NUM_COLS != 0)
			{
				game_Board_Squares[current_Square] -> neighbor[3] = game_Board_Squares[(current_Square- 1)];
			}
			//Right Neighbor
			if(current_Square % NUM_COLS != NUM_COLS-1)
			{
				game_Board_Squares[current_Square] -> neighbor[4] = game_Board_Squares[(current_Square+ 1)];
			}
			//Lower Left Neighbor
			if((current_Square / NUM_ROWS != NUM_ROWS-1) && (current_Square % NUM_COLS != 0) )
			{
				game_Board_Squares[current_Square] -> neighbor[5] = game_Board_Squares[(current_Square + NUM_COLS - 1)];
			}
			//Lower Neighbor
			if(current_Square / NUM_ROWS != NUM_ROWS-1)
			{
				game_Board_Squares[current_Square] -> neighbor[6] = game_Board_Squares[(current_Square + NUM_COLS)];
			}
			//Lower Right Neighbor
			if((current_Square / NUM_ROWS != NUM_ROWS-1) && (current_Square % NUM_COLS != NUM_COLS-1))
			{
				game_Board_Squares[current_Square] -> neighbor[7] = game_Board_Squares[(current_Square + NUM_COLS + 1)];
			}
	}
}

void printBoard() // prints out the grid of the game board when debug enabled
{
#if DEBUG_PRINTING_ON
	for(int i = 0; i < NUM_COLS; i++)
	{
		for(int j = 0; j < NUM_ROWS; j++)
		{
			printf("%c ", *game_Grid_Pointer);
			game_Grid_Pointer++;
		}
		printf("\n");
	}
	game_Grid_Pointer = game_Grid;

#endif
}
void printWords() // prints out all found words when debug enabled
{
#if DEBUG_PRINTING_ON
	word_List_Pointer = topOfWordList;
	while(word_List_Pointer -> word != 0x00)
	{
		for(int i=0;i<strlen(word_List_Pointer -> word);i++)
		{
			printf("%c",word_List_Pointer -> word[i]);
			if(word_List_Pointer -> word[i] == 'Q')
			{
				printf("U");
			}
		}
		if(word_List_Pointer -> nextWord != 0x00)
		{
			printf("\n");
			word_List_Pointer = word_List_Pointer -> nextWord;
		}
		else
		{
			printf("\n");
			break;
		}
	}
#endif
}

void searchForWords() // looks through game board for possible words 
{
	//For each position on the grid check all branching path possibilities
	for(char i = 0; i < NUM_DICE; i++)
	{
		checkGridForWords(game_Board_Squares[i],0);
	}
}

//Taking in starting point on grid as well as remaining squares to check
//Recursively goes through all neighboring squares, checking all word possiblities
//Until it shrivels out 
bool checkGridForWords(grid_Neighbor * position_On_Grid,char remaining_Squares)
{
	//When entering this function add the letter of the square to the test string
	word_length_Counter++;
	*possible_Word_Pointer = position_On_Grid -> letter;
	possible_Word_Pointer++;
	*possible_Word_Pointer = 0X00;	
	
	//To prevent the search from reentering this square mark that it's been visited
	position_On_Grid -> visited = true;

	//Only for strings of at least 2 characters to determine if the string is potentially in dictionairy
	if(word_length_Counter > 1)
	{
		//If there is no word possiblity found break now to speed up
		if(!checkIfDictionaryWord(possible_Word))
		{					
			position_On_Grid -> visited = false;
			return false;
		}
	}
	
	//For each remaining possible square on the grid
	for(int i = 0; i < ((NUM_DICE-remaining_Squares)); i++)
	{
		//For each possible neighbor of the current square
		for(int j = 0; j < MAX_POSSIBLE_GRID_NEIGHBORS; j++)
		{
			//If it has a neighbor
			if((position_On_Grid -> neighbor[j] != 0))
			{
				//And hasn't been visited
				if(position_On_Grid -> neighbor[j] -> visited != true)
				{	
					//Check that neighboring square
					checkGridForWords(position_On_Grid -> neighbor[j],remaining_Squares+1);
					//When this point is reached one of the recursive calls has returned
					//Remove the letter in that was last added to the test string before moving on to next one
					word_length_Counter--;
					possible_Word_Pointer--;
					*possible_Word_Pointer = 0X00;
				}
			}
		}
		//Only reached if all other squares have been checked, make sure to remove the initial letter of test string
		//Before leaving function call
		if(remaining_Squares==0)
		{
			word_length_Counter--;
			possible_Word_Pointer--;
			*possible_Word_Pointer = 0X00;
		}
		position_On_Grid -> visited = false;
		return false;
	}
	position_On_Grid -> visited = false;
	return false;
}

//Test a string constructed from the game board with the dictionary
bool checkIfDictionaryWord(char* check_Word)
{
	node* dictionary_Pointer = dictionary_Struct;
	
	for (unsigned int i = 0; i < strlen(check_Word); i++)
	{
		//Move the pointer to first dictionary reference of check_Word
		dictionary_Pointer = dictionary_Pointer ->  next_Letter[check_Word[i]-'A'];
		//To handle 'QU' dice increment and test it immediately before deciding if need to end search
		if(check_Word[i] == 'Q')
		{
			if(dictionary_Pointer -> next_Letter['U'-'A'] != 0x00)
			{
				dictionary_Pointer = dictionary_Pointer ->  next_Letter['U'-'A'];
			}
			else
			{
				return false;
			}
		}
		//If the check_Word string is found as word in dictionary, and is end of string add it to word_List
		if(dictionary_Pointer -> is_Word == true && (i == (strlen(check_Word)-1)))
		{
			strcpy(words_Found_Pointer,check_Word);
			word_List_Pointer -> nextWord = pushWordList(words_Found_Pointer); 
			word_List_Pointer = word_List_Pointer -> nextWord;
			words_Found_Pointer += (strlen(check_Word)+1);
			return true;
		}
		
		//If the character is marked as last letter in branch but not a word, the search is over
		if(dictionary_Pointer ->is_Last_Letter == true )
		{
			return false;
		}

		//To check if the character in check_Word is actually connected to the next one, determine if this isn't the last character in string
		if(check_Word[i+1] != 0x00)
		{
			//If the connection between the two is undefined search fails
			if(dictionary_Pointer ->next_Letter[check_Word[i+1]-'A'] == 0x00)
			{
				return false;
			}
		}
	}
	//If all the checks are passed through then the string isn't a word itself, but could be part of a larger word
	return true;
}

int32_t rangedRandom(int32_t min, int32_t max) 
{
	return 0;
}


