#ifdef BOGGLE_CPP
#define extern 
#endif

typedef struct node 
{
	char val;
	node * next_Letter[26];
	bool is_Word;
	bool is_Last_Letter;
} node;
typedef struct grid_Neighbor
{
	char letter;
	grid_Neighbor* neighbor[MAX_POSSIBLE_GRID_NEIGHBORS];
	bool visited;
} gridNeighbor;

extern node * dictionary_Struct;
extern grid_Neighbor * head_Of_Grid;

node* createNode(char value);
grid_Neighbor* createGrid(char* game_dice);
bool checkGridForWords(grid_Neighbor * position_On_Grid,char remaining_Squares);
bool checkIfDictionaryWord(char* check_Word);
//

extern grid_Neighbor* square1;
extern grid_Neighbor* square2;
extern grid_Neighbor* square3;
extern grid_Neighbor* square4;
extern grid_Neighbor* square5;
extern grid_Neighbor* square6;
extern grid_Neighbor* square7;
extern grid_Neighbor* square8;
extern grid_Neighbor* square9;
extern grid_Neighbor* square10;
extern grid_Neighbor* square11;
extern grid_Neighbor* square12;
extern grid_Neighbor* square13;
extern grid_Neighbor* square14;
extern grid_Neighbor* square15;
extern grid_Neighbor* square16;
#if  BIG_BOGGLE
extern grid_Neighbor* square17;
extern grid_Neighbor* square18;
extern grid_Neighbor* square19;
extern grid_Neighbor* square20;
extern grid_Neighbor* square21;
extern grid_Neighbor* square22;
extern grid_Neighbor* square23;
extern grid_Neighbor* square24;
extern grid_Neighbor* square25;
#endif

#undef extern