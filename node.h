#pragma once
#include <cstring>
#include <iostream>

class Node
{
	Node** children;
	int*	keys;
	int len;	 // current length of the key
	bool leaf; // Is true when node is leaf. Otherwise false

	Node(int maxKeys, bool _leaf);
	void insert(int k, int maxKeys);
	Node* search(int k); 
	void traverse(int level);
	bool splitChild(int i, int maxKeys);
	void insertKeyAt(int index, int key); 
	void insertChildAt(int index, Node* child);
	int find(int key); 
	bool remove(int k);

friend class BTree;
};
