#pragma once
#define DELIMITER "\n ,.;:?!-\t'()[]{}<>~_"
#define EXPENSIVE '1'
#define PRICE1 1
#define PRICE2 2
typedef struct
{
	unsigned short insId;//instrument's id
	float price; //instrument's price
} MusicianPriceInstrument;
typedef struct listNode
{
	MusicianPriceInstrument* data;
	struct listNode* next;
}ListNode;
typedef struct mpiList
{
	ListNode* head;
	ListNode* tail;
}MPIList;
typedef struct ListnodeSC
{
	MusicianPriceInstrument* data;
	int size_name; //size of how many names in array
	char** name; //array of names
	struct ListnodeSC* next;
}ListNodeSC;
typedef struct
{
	ListNodeSC* head;
	ListNodeSC* tail;
	bool isfound;//indicates success of assigning musicians
	float total; //total cost for the show
}schedualeList;
typedef struct
{
	int size_name;//size of how many names in array
	char** name;//array of names
	MPIList instruments; //list of instruments
	bool busy; //indicates if the musician is already chosen
} Musician;
void makeEmptyList(MPIList* list);
bool isEmptyList(MPIList* list);
void addNodetoEnd(MPIList* list, MusicianPriceInstrument* data, ListNode* next);
ListNode* makeNode(MusicianPriceInstrument* data, ListNode* next);
void freeList(MPIList* list);
int buildArrayMusicians(Musician** MusiciansGroup, InstrumentTree treeInst, char* fileName);
int howManyWords(char* musicData, int* countNames, InstrumentTree tree);
void insertWordstoArray(char** instruments, char* data, int start, int end);
int * fillCollection(Musician*** array, InstrumentTree tree, Musician** data, int numberOfinstruments, int numberOfMusicians);
float getPrice(Musician* mu, int id);
void makeEmptyListSC(schedualeList* list);
bool isEmptyListSC(schedualeList* list);
void addNodetoEndSC(Musician* musician, schedualeList* list, int id, float price, ListNodeSC* next);
ListNodeSC* makeNodeSC(Musician* musician, MusicianPriceInstrument* mpi, ListNodeSC* next);
void freeListSC(schedualeList* list);
void freeArray(char** name, int size);
void freeMusicians(Musician** MusiciansGroup, Musician*** MusiciansCollection, int numberOfMusicians, int numberOfInstruments);