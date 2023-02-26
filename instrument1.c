#include "insHeader.h"
#include "musHeader.h"
#include "showHeader.h"
InstrumentTree BuildInsTree(char* fileName,int * count) 
//gets as char * the filename and the location to store the amount of instruments.
//the functions builds instrument tree and returns the tree and the amount of instruments.
{
	InstrumentTree tree;
	tree.root = NULL;
	FILE* file = fopen(fileName, "rt");
	int uniqueID;
	char name[MAX_LETTERS];
	*(count) = 0;
	if (file == NULL)
	{
		printf("Error! Could not open the file %s", fileName);
		exit(1);
	}
	while (!feof(file))
	{
		uniqueID = (*(count)+1);
		fscanf(file, "%s\n",name);
		addNodeToTree(uniqueID, name, &tree);
		(*(count))++;
	}
	fclose(file);
	return tree;
}
void addNodeToTree(int id, char* name, InstrumentTree* tree)
//Wrapper function:gets id as integer,name as char * , tree as instrument * 
//the function adds node with the requsted data in the correct place.
{
	if (tree->root == NULL)
	{
		tree->root = (TreeNode*)malloc(sizeof(TreeNode));
		checkAllocation((void*)tree->root);
		tree->root->insID = id;
		copyInstrumentName(tree->root, name);
		tree->root->left = tree->root->right = NULL;
	}
	else
		addNodeToTreeRec(id, name, tree->root);
}
void addNodeToTreeRec(int id, char* name, TreeNode* tr)
//Recursive function:gets id as integer,name as char * , tr as TreeNode * 
//the function adds node in the correct place by the order of the alphabet.
{
	int compare = strcmp(name, tr->instrument);
	if (compare > 0 && tr->right == NULL)
		addNode(id, name, tr, RIGHT);
	else if (compare < 0 && tr->left == NULL)
		addNode(id, name, tr, LEFT);
	else
	{
		if (compare > 0)
			addNodeToTreeRec(id, name, tr->right);
		else
			addNodeToTreeRec(id, name, tr->left);
	}
}
void addNode(int id, char* name, TreeNode* tr, int direction)
//Recursive function:gets id as integer,name as char * , tr as TreeNode * ,direction as integer
//function creates a node with the requseted data and places it in the requsted direction
{
	TreeNode* newNode;
	newNode = (TreeNode*)malloc(sizeof(TreeNode));
	checkAllocation((void*)newNode);
	newNode->insID = id;
	copyInstrumentName(newNode, name);
	newNode->left = newNode->right = NULL;
	if (direction == RIGHT)
		tr->right = newNode;
	else
		tr->left = newNode;
}
void checkAllocation(void* ptr)
//gets ptr as void * and checks if the allocation faild and acts accorodingly
{
	if (ptr == NULL)
	{
		printf("Allocation failed!");
		exit(1);
	}
}
void copyInstrumentName(TreeNode* tr, char* name)
//function gets tr as TreeNode * ,name as char *.the function copy the name into names's treeNode.
{
	int length = strlen(name);
	tr->instrument = (char*)malloc((length+1) * sizeof(char));
	checkAllocation((void*)tr->instrument);
	strcpy(tr->instrument, name);
}
int findInsId(InstrumentTree tree, char* instrument)
//Wrapper function:the function gets tree as InstrumentTree, instrument as char * and returns the id of instrument realated to the requsted name
{
	return findInsIdRec(tree.root, instrument);
	 
}
int findInsIdRec(TreeNode* tr, char* instrument)
//Recursive funcion: the function gets tr as TreeNode * ,instrument as char *  and returns the id of instrument realated to the requsted name
{
	int compare;
	if (tr != NULL)
	{
		compare=strcmp(instrument, tr->instrument);
		if (compare == 0)
			return tr->insID;
		else if (compare > 0)
			return findInsIdRec(tr->right, instrument);
		else
			return findInsIdRec(tr->left, instrument);
	}
	else
		return NOT_FOUND;
}
void printdInsId(TreeNode* tr, int id)
//Recursive function:gets tr as TreeNode * and id as integer and prints the name of instrument related to the requsted id
{
	if (tr != NULL)
	{
		if (tr->insID == id)
			printf("- %s ", tr->instrument);
		else
		{
				printdInsId(tr->right, id);
				printdInsId(tr->left, id);
		}
	}
}
void freeTree(TreeNode* tr)
//Recursive function: gets tr as TreeNode* and function frees all allocations that were made for the instrument tree
{
	if (tr != NULL)
	{
		freeTree(tr->left);
		freeTree(tr->right);
		free(tr->instrument);
		free(tr);
	}
}