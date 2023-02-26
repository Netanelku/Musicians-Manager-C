#include "insHeader.h"
#include "musHeader.h"
#include "showHeader.h"

Concert* buildConcertsArray(int * sizeOfConcerts, InstrumentTree treeInst)
//function gets a location to store the size of concerts as int * ,treeinst as instrumentTree
//and the function builds array of concerts and returns it as Concert *
{
	int physicalSize = INITIAL_SIZE_CONCERT,logicalSize=0;
	int line_size=0;
	Concert* array = (Concert*)malloc(physicalSize* sizeof(Concert));
	checkAllocation((void*)array);
	char* dataLine = getData(&line_size);
	while (line_size!=0)
	{
		if (logicalSize == physicalSize)
		{
			physicalSize *= INCREASE_SIZE;
			array = (Concert*)realloc(array,physicalSize*sizeof(Concert));
			checkAllocation((void*)array);
		}
		makeConcert(array + logicalSize, dataLine, treeInst);
		free(dataLine);
		logicalSize++;
		dataLine = getData(&line_size);
	}
	if (logicalSize != physicalSize)
	{
		array = (Concert*)realloc(array, logicalSize * sizeof(Concert));
		checkAllocation((void*)array);
	}
	*sizeOfConcerts = logicalSize;
	return array;
}

char* getData(int* size)
//function gets  a location to store the size as int * ,function gets input from the user till \n and returns a string as char * 
{
	int logicalSize = 0, physicalSize = INITIAL_SIZE_LINE;
	char ch1;
	*size = 0;
	char* array = (char*)malloc(physicalSize * sizeof(char));
	checkAllocation((void*)array);
	ch1 = getchar();
	while (ch1 != '\n')
	{
		if (logicalSize == physicalSize)
		{
			physicalSize *= INCREASE_SIZE;
			array = (char*)realloc(array, physicalSize * sizeof(char));
			checkAllocation((void*)array);
		}
		*(array + logicalSize) = ch1;
		logicalSize++;
		ch1 = getchar();
	}
	if ((logicalSize+1) != physicalSize)
	{
		array = (char*)realloc(array, (logicalSize+1) * sizeof(char));
		checkAllocation((void*)array);
	}
	array[logicalSize] = '\0';
	*size = logicalSize;
	return array;
}
void makeConcert(Concert* concert, char* data, InstrumentTree treeInst)
//function gets concert as concert *, data as char * , treeInst as instrument tree.
//function updates data for the concert struct .(name,date,instruments)
{
	Concert temp;
	temp.name = getNameShow(data);
	temp.date_of_concert = getDateShow(data);
	getInstruments(&temp,data, treeInst);
	*concert = temp;
	
}
void getInstruments(Concert* concert, char* data, InstrumentTree treeInst)
//function gets concert as concert *, data as char * , treeInst as instrument tree.
//the function updates the concert with the requsted instruments
{
	CIList list;
	ConcertInstrument temp;
	makeEmptyListCI(&list);
	int i;
	char* copy = (char*)malloc((strlen(data) + 1) * sizeof(char));
	checkAllocation((void*)copy);
	strcpy(copy, data);
	char* token = strtok(copy, " :");
	for (i = 0; i < 6; i++)
		token = strtok(NULL, " :");
	while (token != NULL)
	{
		temp.inst = findInsId(treeInst, token);
		token = strtok(NULL, " :");
		temp.num = atoi(token);
		token= strtok(NULL, " :");
		temp.importance = token[0];
		addNodetoEndCI(&list, temp, NULL);
		token = strtok(NULL, " :");
	}
	free(copy);
	(*concert).instruments = list;
}
char* getNameShow(char* data)
//gets data as char * and returns the name of the show as char *.
{
	char* copy = (char*)malloc((strlen(data)+1) * sizeof(char));
	checkAllocation((void*)copy);
	strcpy(copy, data);
	char* token = strtok(copy, " ");
	char * name = (char*)malloc((strlen(token)+1) * sizeof(char));
	checkAllocation((void*)name);
	strcpy(name, token);
	free(copy);
	return name;
}
Date getDateShow(char* data)
//gets data as char * and returns the date of the show as Date struct.
{
	Date date;
	char* copy = (char*)malloc((strlen(data)+1)* sizeof(char));
	checkAllocation((void*)copy);
	strcpy(copy, data);
	char* token = strtok(copy, " :");
	token = strtok(NULL," :");
	date.day = atoi(token);
	token = strtok(NULL, " :");
	date.month = atoi(token);
	token = strtok(NULL, " :");
	date.year= atoi(token);
	token = strtok(NULL, " :");
	date.hour= atoi(token);
	token = strtok(NULL, " :");
	date.hour += atoi(token)/(float)TIME_SPLIT;
	free(copy);
	return date;
}
void makeEmptyListCI(CIList* list)
//gets list as CIList * and initialize it with dummy head
{
	ConcertInstrument nullInstrument = { 0,0,0 };
	list->head = list->tail = makeNodeCI(nullInstrument, NULL);
}
bool isEmptyListCI(CIList* list)
//gets list as CIList * and return boolean regrading list emptiness
{
	return (list->head->next == NULL);
}
void addNodetoEndCI(CIList* list, ConcertInstrument data, ciListNode* next)
//gets list as CIList * ,data as concertInstrument,next as ciListNode * and the function add new node to the end to the list.
{
	ciListNode* newNode = makeNodeCI(data, next);
	list->tail->next = newNode;
	list->tail = newNode;
}
ciListNode* makeNodeCI(ConcertInstrument data, ciListNode* next)
//gets data as concertInstrument,next as ciListNode * and the function creates new node and returns it as ciListNode *.

{
	ciListNode* newNode = (ciListNode*)malloc(sizeof(ciListNode));
	checkAllocation((void*)newNode);
	newNode->data = data;
	newNode->next = next;
	return newNode;
}
void freeListCI(CIList* list)
//gets list as CIList * and the function frees its elements
{
	ciListNode* next, * current = list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	list->head = list->tail = NULL;
}
void ScheduleMusicians(Concert* show,Musician*** collection, int* counterMusicians, 
	int numberOfConcerts, Musician** MusiciansGroup,int musicians,InstrumentTree tree)
	/*functions gets show as concert*, collection as Musician***, counter as int*,number of concerts as integer,musiciangroup as Musician **
	* musicians as integer and tree as instrument tree.
	* the function sorts collection array and assigns musicians for every concert if it is posiible and then prints accorodingly.*/
{
	int i;
	schedualeList temp;
	for (i = 0; i < numberOfConcerts; i++)
	{
		SortWrapper(collection, *(show + i), counterMusicians);
		getMu(collection, *(show + i), counterMusicians, &temp);
		if (!temp.isfound)
			printf("Could not find musicians for the concert %s\n", (*(show+i)).name);
		else
		{
			printf("%s ", (*(show + i)).name);
			printDate((*(show+i)).date_of_concert);
			printInstruments(&temp,tree);
			printf("Total cost: %d.\n",(int)(temp.total));
			freeListSC(&temp);
		}
		intializeBusy(musicians, MusiciansGroup);
	}
}
void printDate(Date date)
//function gets date as Date struct and prints its elements
{
	int hour, min;
	hour = ((int)(date.hour * TIME_SPLIT)) / TIME_SPLIT;
	min = ((int)(date.hour * TIME_SPLIT)) % TIME_SPLIT;
	printf("%02d %02d %04d %02d:%02d: ",date.day,date.month,date.year,hour,min);
}
void intializeBusy(int musicians, Musician** MusiciansGroup)
//function gets musicians as integer and MusiciansGroup as Musician **.
//the functions over each musician initialize BUSY element to false
{
	int i;
	for (i = 0; i < musicians; i++)
	{
		(*(MusiciansGroup + i))->busy = false;
	}
}
void printInstruments(schedualeList* list,InstrumentTree tree)
//function gets a list as schedualeList* ,tree as instrumentTree
//the fucntions prints the elements of the list(instrument,prica,name)
{
	ListNodeSC* current = list->head->next;
	int i;
	bool start = true;
	while (current != NULL)
	{
		for (i = 0; i<current->size_name; i++)
		{
			printf("%s ", *((current->name) + i));
		}

		printdInsId(tree.root,current->data->insId);
		if(current->next==NULL)
			printf("(%d). ", (int)current->data->price);
		else
			printf("(%d), ", (int)current->data->price);
		current = current->next;
	}
}
void getMu(Musician*** collection, Concert show, int* counterMusicians, schedualeList* temp)
//functions get collection as Musician ***, show as Concert struct, counterMusicians as int* ,temp as schedualeList*
//The function is responsible for allocate Musicians for the given show.
{
	ciListNode* current; int id, quantity,i,count;
	current = show.instruments.head->next; Musician* musician;
	temp->isfound = true; float price;
	makeEmptyListSC(temp);
	while (current != NULL)
	{
		id= current->data.inst;
		quantity = current->data.num;
		if (quantity > counterMusicians[id - 1])
		{
			temp->isfound = false;
			break;
		}
		temp->isfound = false; count = 0;
		for (i = 0; i < counterMusicians[id - 1]; i++)
		{
			musician = (*((*(collection + id - 1)) + i));
			if (musician->busy == false)
			{
				musician->busy = true;
				count++;
				price = getPrice(musician, id);
				temp->total += price;
				addNodetoEndSC(musician,temp,id,price,NULL);
			}
			if (count == quantity)
			{
				temp->isfound = true;
				break;
			}
		}
		if (temp->isfound == false)
			break;
		current = current->next;
	}
}
void freeConcerts(Concert * concerts, int numberOfConcerts)
//gets concerts as Concert *, numberOfConcerts as integer
//the function frees concerts elements.
{
	int i;
	for (i = 0; i < numberOfConcerts; i++)
	{
		free((*(concerts + i)).name);
		freeListCI(&(*(concerts + i)).instruments);
	}
}

int fromExpensive(const void** p1, const void** p2)
//gets two musicians as const void ** and returns who is more expensive by integer
{
	const Musician** t1 = p1;
	const Musician** t2 = p2;
	float price1 = getPrice(*t1, globalId);
	float price2 = getPrice(*t2, globalId);
	return (int)price2 - (int)price1;
}
int toExpensive(const void** p1, const void** p2)
//gets two musicians as const void ** and returns who is more expensive by integer
{
	const Musician** t1 = p1;
	const Musician** t2 = p2;
	float price1 = getPrice(*t1, globalId);
	float price2 = getPrice(*t2, globalId);
	return (int)price1 - (int)price2;
}