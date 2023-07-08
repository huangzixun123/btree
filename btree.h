#pragma once
#include "node.h"
// A BTree
class BTree
{
	Node* root;
	int degree;
public:
	BTree(int _degree);
	void Traverse();
	Node* Search(int k);
	void ReplaceOrInsert(int k);
private:
	int maxKeys();
	int minKeys();
};

