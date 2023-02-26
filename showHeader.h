#pragma once
#define INITIAL_SIZE_CONCERT 1
#define INITIAL_SIZE_LINE 1
#define TIME_SPLIT 100
#define INCREASE_SIZE 2
typedef struct
{
	int day, month, year;
	float hour;
} Date;
typedef struct
{
	int num;//quantity
	int inst;//id
	char importance; //importance for expensive
} ConcertInstrument;
typedef struct ciListnode
{
	ConcertInstrument data; // date
	struct ciListnode* next;
}ciListNode;
typedef struct Cilist
{
	ciListNode* head;
	ciListNode* tail;
}CIList;
typedef struct
{
	Date date_of_concert;  //date
	char* name; //name
	CIList instruments; // instruments
} Concert;
int globalId;
Concert* buildConcertsArray(int* sizeOfConcerts, InstrumentTree treeInst);
char* getData(int* size);
Date getDateShow(char* data);
char* getNameShow(char* data);
void getInstruments(Concert* concert, char* data, InstrumentTree treeInst);
void makeConcert(Concert* concert, char* data, InstrumentTree treeInst);
void makeEmptyListCI(CIList* list);
bool isEmptyListCI(CIList* list);
void addNodetoEndCI(CIList* list, ConcertInstrument data, ciListNode* next);
ciListNode* makeNodeCI(ConcertInstrument data, ciListNode* next);
void freeListCI(CIList* list);
void ScheduleMusicians(Concert* show, Musician*** collection, int* counterMusicians, 
	int numberOfConcerts, Musician** MusiciansGroup, int musicians, InstrumentTree tree);
void SortWrapper(Musician*** collection, Concert show, int* counterMusicians);
void intializeBusy(int musicians, Musician** MusiciansGroup);
void getMu(Musician*** collection, Concert show, int* counterMusicians, schedualeList* temp);
void printInstruments(schedualeList* list, InstrumentTree tree);
void printDate(Date date);
void freeConcerts(Concert* concerts, int numberOfConcerts);
int fromExpensive(const void** p1, const void** p2);
int toExpensive(const void** p1, const void** p2);