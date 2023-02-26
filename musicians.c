#include "insHeader.h"
#include "musHeader.h"
#include "showHeader.h"

void makeEmptyList(MPIList* list)
//gets list as MPIList* and initializes it with dummy head
{
	list->head=list->tail = makeNode(NULL,NULL);
}
bool isEmptyList(MPIList* list)
//gets list as MPIList* and checks if the list is empty
{
	return(list->head->next = NULL);
}
void addNodetoEnd(MPIList* list, MusicianPriceInstrument* data,ListNode * next)
//gets list as MPIList* and data as MusicianPriceInstrument*and next as ListNode *
//functions creates a node with the requsted data and inserts it to the end of the list
{
	ListNode* newNode = makeNode(data, next);
	list->tail->next = newNode;
	list->tail = newNode;
}
ListNode* makeNode(MusicianPriceInstrument* data, ListNode* next)
//gets data as MusicianPriceInstrument*and next as ListNode *
//function creates a node and returns it as ListNode *
{
	ListNode* newNode = (ListNode *)malloc(sizeof(ListNode));
	checkAllocation((void*)newNode);
	newNode->data = data;
	newNode->next = next;
	return newNode;
}
void freeList(MPIList* list)
//gets list as MPIList* and frees its elements
{
	ListNode*next,* current = list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
	list->head = list->tail = NULL;
}
int buildArrayMusicians(Musician** MusiciansGroup, InstrumentTree treeInst, char* fileName)
//gets musicianGroup as Musician**,treeInst as InstrumentTree, filename as char*
//the functions builds array of musicians and returns as integer the amount of musicians
{
	FILE* file = fopen(fileName, "rt");
	MusicianPriceInstrument * mpiTemp;char musicianData[MAX_LETTERS];
	char** arrayNames,**arrayInstruments;int length,countNames,count=0,i;
	if (file == NULL)
	{
		printf("Error! Could not open the file %s", fileName);
		exit(1);
	}
	while (!feof(file))
	{
		fgets(musicianData, MAX_LETTERS, file);
		length = howManyWords(musicianData,&countNames, treeInst);
		arrayNames = (char**)malloc(countNames*sizeof(char *));
		checkAllocation((void*)arrayNames);
		arrayInstruments= (char**)malloc((length-countNames) * sizeof(char*));
		checkAllocation((void*)arrayInstruments);
		insertWordstoArray(arrayNames, musicianData, 0, countNames);
		insertWordstoArray(arrayInstruments, musicianData, countNames, length);
		*(MusiciansGroup + count) = (Musician*)malloc(sizeof(Musician));
		checkAllocation((void*)*(MusiciansGroup + count));
		makeEmptyList(&(*(MusiciansGroup + count))->instruments);
		for (i = 0; i < length - countNames; i += 2)
		{
			mpiTemp = (MusicianPriceInstrument*)malloc(sizeof(MusicianPriceInstrument));
			checkAllocation((void*)mpiTemp);
			mpiTemp->insId = findInsId(treeInst, *(arrayInstruments + i));
			mpiTemp->price = (float)atoi(*(arrayInstruments + i+1));
			free(*(arrayInstruments + i));
			addNodetoEnd(&(*(MusiciansGroup + count))->instruments, mpiTemp, NULL);
		}
		free(arrayInstruments);
		((*(MusiciansGroup + count))->name) = arrayNames;
		((*(MusiciansGroup + count))->size_name) = countNames;
		(*(MusiciansGroup + count))->busy = false;
		count++;
	}
	fclose(file);
	return count;
}
int howManyWords(char* musicData,int * countNames,InstrumentTree tree)
//gets musicData as char * , countNames as int *,tree as InstrumentTree
//function count how many names and words in the given char* musicData and returns it as integer
{
	int count = 0;
	char array[MAX_LETTERS];
	strcpy(array, musicData);
	char* token = strtok(array, DELIMITER);
	bool isName = true;
	*(countNames) = 0;
	while (token != NULL)
	{

		if (findInsId(tree, token) != -1)
			isName = false;
		if (findInsId(tree, token) == -1 && isName == true)
			(*countNames)++;
		count++;
		token=strtok(NULL, DELIMITER);
	}
	return count;
}
void insertWordstoArray(char** instruments, char* data, int start, int end)
//function gets instruments as char ** ,data as char *, two integers(start,end)
//function inserts spesific words into instruments array
{
	int count = 0;
	char array[MAX_LETTERS];
	strcpy(array, data);
	char* token = strtok(array, DELIMITER);
	while (token != NULL)
	{
		if (start > 0)
			start--;
		else if (end - start > 0)
		{
			*(instruments + count) = (char*)malloc((strlen(token)+1) * sizeof(char));
			checkAllocation((void*)*(instruments + count));
			strcpy(*(instruments + count), token);
			count++;
			end--;
		}
		token=strtok(NULL, DELIMITER);
	}
}
int * fillCollection(Musician*** array, InstrumentTree tree,Musician ** data,int numberOfinstruments,int numberOfMusicians)
//function gets array as Musician ***,tree as instrumentTree,data as Musician **,two integers(numberOfinstruments,numberOfMusicians)
//functions fills array with pointers to musicians, and return how many pointers in every sub arr in array.
{
	int* countInstrument1 = (int *)calloc(numberOfinstruments,sizeof(int));
	checkAllocation((void*)countInstrument1);
	int* countInstrument2 = (int*)calloc(numberOfinstruments, sizeof(int));
	checkAllocation((void*)countInstrument2);
	int i, id;
	ListNode* current;
	for (i = 0; i < numberOfMusicians; i++)
	{
		current = (*(data + i))->instruments.head->next;
		while (current != NULL)
		{
			 id = current->data->insId;
			(*(countInstrument1 + id-1))++;
			(*(countInstrument2 + id - 1))++;
			current = current->next;
		}
	}
	for (i = 0; i < numberOfinstruments; i++)
	{
		*(array + i) = (Musician**)malloc(countInstrument1[i] * sizeof(Musician*));
		checkAllocation((void*)*(array + i));
		countInstrument1[i] = 0;
		
	}
	for (i = 0; i < numberOfMusicians; i++)
	{
		current = (*(data + i))->instruments.head->next;
		while (current != NULL)
		{
			 id = current->data->insId;
			 *((*(array + (id - 1))) + countInstrument1[id - 1]) = *(data + i);
			(countInstrument1[id-1])++;
			current = current->next;
		}
	}
	free(countInstrument1);
	return countInstrument2;
}
int howManyMusicians(char* filename)
//gets filename as char * ,the fucntion counts how many musicians as integer
{
	FILE* fl = fopen(filename, "rt");
	if (fl == NULL)
	{
		printf("Error! Could not open the file %s", filename);
		exit(1);
	}
	int count = 0;
	char line[MAX_LETTERS];
	while (!feof(fl))
	{
		fgets(line, MAX_LETTERS, fl);
		count++;
	}
	fclose(fl);
	return count;
}
void SortWrapper(Musician*** collection, Concert show, int* counterMusicians)
//Wrapper function:gets collection as Musician ***, show as concerts, counterMusicians as int*
//the function sorts the sub arrays of collection by the given show
{
	ciListNode* current = show.instruments.head->next;
	char expensive;
	int id;
	while (current != NULL)
	{
		expensive = current->data.importance;
		id = current->data.inst;
		globalId = id;
		if (expensive == EXPENSIVE)
			qsort(*(collection + id - 1), counterMusicians[id - 1], sizeof(Musician*), fromExpensive);
		else
			qsort(*(collection + id - 1), counterMusicians[id - 1], sizeof(Musician*), toExpensive);
		current = current->next;
	}
}
float getPrice(Musician* mu, int id)
//gets mu as Musician* and id as integrs.function returns as float the price for the instrument by given id
{
	ListNode* current = mu->instruments.head->next;
	while (current != NULL)
	{
		if (current->data->insId == id)
			break;
		current = current->next;
	}
	return current->data->price;
}
void makeEmptyListSC(schedualeList* list)
//gets a list as schedualeList* and intializes it with dummyhead
{
	Musician* mu= (Musician*)malloc(sizeof(Musician));
	checkAllocation((void*)mu);
	MusicianPriceInstrument* new = (MusicianPriceInstrument*)malloc(sizeof(MusicianPriceInstrument));
	checkAllocation((void*)new);
	new->insId = 0;
	new->price = 0;
	mu->busy = false;
	mu->name = NULL;
	makeEmptyList(&mu->instruments);
	mu->size_name = 0;
	list->head = list->tail = makeNodeSC(mu,new,NULL);
	list->total = 0;
}
bool isEmptyListSC(schedualeList* list)
//function gets a list as schedualeList* and checks if the list is empty
{
	return(list->head->next = NULL);
}
void addNodetoEndSC(Musician* musician, schedualeList* list, int id,float price, ListNodeSC* next)
//function gets musician as Musician* ,list as schedualeList *, id as integer,price as float,next as ListNodeSC*
//the function creates a node and adds it to the end of the list
{
	MusicianPriceInstrument* new = (MusicianPriceInstrument*)malloc(sizeof(MusicianPriceInstrument));
	checkAllocation((void *)new);
	new->insId = id;
	new->price = price;
	ListNodeSC* newNode = makeNodeSC(musician, new, next);
	list->tail->next = newNode;
	list->tail = newNode;
}
ListNodeSC* makeNodeSC(Musician* musician, MusicianPriceInstrument* mpi, ListNodeSC* next)
//function gets musician as Musician* ,mpi as MusicianPriceInstrument *,next as ListNodeSC*
//the function creates a new Node with the given data and returns it as ListNodeSC*
{
	ListNodeSC* newNode = (ListNodeSC*)malloc(sizeof(ListNodeSC));
	checkAllocation((void*)newNode);
	newNode->name = musician->name;
	newNode->size_name = musician->size_name;
	newNode->data = mpi;
	newNode->next = NULL;
	return newNode;
}
void freeListSC(schedualeList* list)
//function gets a list as schedualeList* and frees its elements
{
	ListNodeSC* next, * current = list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
	list->head = list->tail = NULL;
}
void freeArray(char** name, int size)
//function gets name as char ** and size as integer and the function frees name's elements
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(*(name + i));
	}
	free(name);
}
void freeMusicians(Musician** MusiciansGroup, Musician*** MusiciansCollection, int numberOfMusicians, int numberOfInstruments)
//function gets MusciansGroup as Musician**,MusiciansCollection as Musician *** and two integers (num. musician, num. instruments)
//the function frees all allocations that were made in MusiciansGroup and in MusiciansCollection
{
	int i = 0;
	for (i = 0; i < numberOfInstruments; i++)
		free(*(MusiciansCollection + i));
	free(MusiciansCollection);
	for (i = 0; i < numberOfMusicians; i++)
	{
		freeArray((*(MusiciansGroup + i))->name, (*(MusiciansGroup + i))->size_name);
		freeList(&((*(MusiciansGroup + i))->instruments));
		free(*(MusiciansGroup + i));
	}
	free(MusiciansGroup);

}
