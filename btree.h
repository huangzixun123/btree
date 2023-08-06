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
	Node* Get(int k);
	void Put(int k);
	bool Delete(int k);
private:
	int maxKeys();
	int minKeys();
};

