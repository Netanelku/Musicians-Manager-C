#include "Header.h";
InstrumentTree BuildTree(char * file)
{
	InstrumentTree tree;
	FILE * file = fopen(file, 'r');
	checkOpenFile(file);
	tree.root = NULL;
	tree.root = BuildTreeRec(file);
	fclose(file);
}
TreeNode * BuildTreeRec(FILE * file);
void checkOpenFile(void * ptr)
{
	if (ptr == NULL)
	{
		printf("failed to open the text file");
		exit(1);
	}
}
int howManyInstruments(char * fileAdress)
{
	int count = 0;
	char c;
	FILE * file = fopen(fileAdress, 'r');
	while (!feof(file)) {
		fcanf(file, "%c", &c);
		if (c == '\n')
			count++;
	}
	fclose(file);
	return count;
}