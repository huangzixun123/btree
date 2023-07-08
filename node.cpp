#include "node.h"

Node::Node(int maxKeys, bool _leaf)
{
	leaf = _leaf;
	keys = new int[maxKeys];
	children = new Node*[maxKeys+1];
	len = 0;
}

// insert inserts a key into the subtree rooted at this node, making sure
// no nodes in the subtree exceed maxKeys keys.
void Node::insert(int k, int maxKeys)
{
	int i = len-1;

	if (leaf == true)
	{
		// Finds the location of new key to be inserted and
		// Moves all greater keys to one place ahead
		while (i >= 0 && keys[i] > k)
		{
			keys[i+1] = keys[i];
			i--;
		}

		// Insert the new key at found location
		this->keys[i+1] = k;
		this->len = this->len +1;
	} else {
		// Find the child which is going to have the new key
		while (i >= 0 && keys[i] > k)
			i--;

		if (this->children[i+1]->len >= maxKeys)
		{
			splitChild(i+1, maxKeys);
			// After split, the middle key of keys[i] goes up.
			//  See which of the two
			// is going to have the new key
			if (keys[i+1] < k)
				i++;
		}
		children[i+1]->insert(k, maxKeys);
	}
}

// search searchs the given key k in subtree rooted with this node
Node* Node::search(int k)
{
	// Find the first key greater than or equal to k
	int i = 0;
	while (i < len && k > keys[i])
		i++;

	if (keys[i] == k)
		return this;

	if (leaf == true)
		return nullptr;

	return children[i]->search(k);
}

// traverse is used for testing/debugging purposes.
void Node::traverse(int level)
{
	int i;
	std::cout<<"level "<<level<<" : ";
	for (i = 0; i < len; i++)
	{
		std::cout << keys[i] << " ";
	}
	if (leaf == false){
		std::cout<<std::endl;
		for (i = 0; i <= len; i++){
			children[i]->traverse(level+1);
		}
	}	
}

// splitChild splits the given child at the given index. 
// Returns whether or not a split successed.
bool Node::splitChild(int i, int maxKeys)
{
	if(this->children[i]->len < maxKeys){
		return false;
	}
	int degree = (maxKeys + 1) / 2;
	Node* first = this->children[i];
	first->len = degree - 1;

	Node *second = new Node(degree, first->leaf);
	memcpy(second->keys, first->keys + degree, sizeof(int) * (degree-1));
	if (second->leaf == false){
		memcpy(second->children, first->children + degree, sizeof(Node*)*degree);
	}
	second->len = degree - 1;

	this->insertChildAt(i+1, second);
	this->insertKeyAt(i, first->keys[degree-1]);
	len += 1;
}

// inertKeyAt inserts a key into the given index, pushing all subsequent keys
// forward
void Node::insertKeyAt(int index, int key) {
	int j = len;
	for(j=len;j>=index+1;j++){
		keys[j] = keys[j-1];
	}
	keys[index] = key;
}

// inertChildAt inserts a child into the given index, pushing all subsequent children
// forward
void Node::insertChildAt(int index, Node* child) {
	int j = len + 1;
	for(j=len+1;j>=index+1;j++){
		children[j] = children[j-1];
	}
	children[index] = child;
}

// find finds the index of the given key
// Returns the index of the key when the key is found. Otherwise return -1.
int Node::find(int key) {
	int left = 0, right = len - 1;

	while(left <= right){
		int mid = (left + right) / 2;
		if(key > keys[mid]){
			left = mid + 1;
		} else if (key < keys[mid]){
			left = mid-1;
		} else {
			return mid;
		}
	}
	return -1;
}
