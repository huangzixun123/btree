#include "btree.h"

BTree::BTree(int _degree){
	root = nullptr;
	degree = _degree; 
}

void BTree::Traverse(){
	if (root != nullptr) 
		root->traverse(0);
}

Node* BTree::Search(int k){
	return (root == nullptr)? nullptr : root->search(k);
}

// maxKeys returns the max number of keys to allow per node.
int BTree::maxKeys(){
	return 2 * degree - 1;
}

// minKeys returns the min number of keys to allow per node.
int BTree::minKeys(){
	return degree - 1;
}

// ReplaceOrInsert adds the given key k to the tree.
void BTree::ReplaceOrInsert(int k)
{
	if (root == nullptr) {
		root = new Node(maxKeys(), true);
		root->keys[0] = k;
		root->len = 1;
		return;
	} else {
		if (root->len >= maxKeys())
		{
			Node *newroot = new Node(maxKeys(), false);
			newroot->children[0] = root;
			newroot->splitChild(0, maxKeys());
			root = newroot;
		}
	}
	root->insert(k, maxKeys());
}
