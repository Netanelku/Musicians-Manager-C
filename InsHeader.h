#pragma once
#define MAX_LETTERS 150
#define NOT_FOUND -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#define LEFT 1
#define RIGHT 2
typedef struct treeNode {
	char* instrument; //instrument's name
	unsigned short insID;//instrument's id
	struct treeNode* left;
	struct treeNode* right;
}TreeNode;

typedef struct tree {
	TreeNode* root;
}InstrumentTree;
InstrumentTree BuildInsTree(char* file,int * count);
void addNodeToTree(int id, char* name, InstrumentTree* tree);
void addNodeToTreeRec(int id, char* name, TreeNode* tr);
void addNode(int id, char* name, TreeNode* tr, int direction);
void checkAllocation(void* ptr);
void copyInstrumentName(TreeNode* tr, char* name);
int findInsId(InstrumentTree tree, char* instrument);
void printdInsId(TreeNode* tr, int id);
void freeTree(TreeNode* tr);