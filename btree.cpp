#include "btree.h"

BTree::BTree(int _degree){
	root = nullptr;
	degree = _degree; 
}

void BTree::Traverse(){
	if (root != nullptr) 
		root->traverse(0);
}

// Get searchs the given key in the tree
Node* BTree::Get(int k){
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

// Put inserts the given key k to the tree.
void BTree::Put(int k)
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

// Delete deletes the given key k in the tree.
bool BTree::Delete(int k) {
	if (root == nullptr || root.len == 0) {
		return false;
	}

	bool success = root.remove(k);
	if(root.len == 0) {
		Node* oldroot = root;
		if(root.leaf == true){
			root = nullptr;
		}else{
			root = oldroot->children[0];
		}
		delete root;
	}

	return success;
}