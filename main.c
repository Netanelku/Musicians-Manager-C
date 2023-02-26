#include "insHeader.h"
#include "musHeader.h"
#include "showHeader.h"
/*
Author:Netanel Kuchma
ID: 209344365
*/

void main(int argc, char** argv)
{
	Musician** MusiciansGroup;
	int numberOfMusicians, numberOfInstruments,numberOfConcerts;
	int* counterMusicians;
	InstrumentTree treeInst;
	Musician*** MusiciansCollection;
	Concert* concerts;
	if (argc != 3) //checks number of arguments
	{
		printf("Error! The program did not get 3 Arguments");
		exit(1);
	}
	treeInst = BuildInsTree(argv[1],&numberOfInstruments); //builds instruments tree
	numberOfMusicians = howManyMusicians(argv[2]); //get number of musicians as integedr
	MusiciansGroup=(Musician**)malloc(numberOfInstruments * sizeof(Musician*));//allocte memory for musicianGroup
	checkAllocation((void*)MusiciansGroup);
	buildArrayMusicians(MusiciansGroup, treeInst, argv[2]);//builds array of musicians
	MusiciansCollection = (Musician***)malloc(numberOfInstruments * sizeof(Musician**));//allocte memory for MusiciansCollection
	checkAllocation((void*)MusiciansCollection);
	//fills fillcollection array
	counterMusicians=fillCollection(MusiciansCollection,treeInst, MusiciansGroup,numberOfInstruments,numberOfMusicians);
	concerts = buildConcertsArray(&numberOfConcerts,treeInst);//builds array of concerts
	//schedule musicians for concerts
	ScheduleMusicians(concerts, MusiciansCollection, counterMusicians,numberOfConcerts,MusiciansGroup, numberOfMusicians, treeInst);
	freeTree(treeInst.root);//frees instruments Tree
	free(counterMusicians);//frees counter length of arrays of musiciansCollection
	freeMusicians(MusiciansGroup, MusiciansCollection, numberOfMusicians, numberOfInstruments);//frees all allocations regarding musicians
	freeConcerts(concerts, numberOfConcerts);//frees concerts array
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}