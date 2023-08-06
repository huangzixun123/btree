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


// find returns the index where the given item should be inserted into this list.
//'found' is true if the item already exists in the list at the given index.
int Node::find(int key, bool& flag) {
	int i = 0, j = len;
	// f(i) = keys[i] > key
	// Define f(-1) = false and f(n) = true
	// Invariant: f(i-1) == false, f(j) == true
	while(i < j){
		int h = i + (j - i) >> 1;
		if(keys[h] <= key){
			i = h + 1;
		}else{
			j = h;
		}
	}
	if(i > 0 && keys[i-1] >= key){
		flag = true;
		return i-1;
	}
	// i == j, f(i-1) == false, and f(j) (=f(i)) == true => answer is i.
	return i;
}

const uint DEL_KEY_NOT_FOUND = 1 << 63;
enum removeType {removeMax, removeMin, removeKey};
// remove delete the key in given key in the tree
// Returns the removed key indicates remove successed
int Node::remove(int k, int minKeys, typ removeType){
	int i;
	bool found = false;
	switch typ {
		case removeMax:
			if(this->leaf){
				return removeKeyAt(len-1);
			}
			i = len;
			break;
		case removeMin:
			if(this->leaf) {
				return removeKeyAt(0);
			}
			i = 0;
			break;
		case removeKey:
			i = find(k, &found);
			if(this->leaf){
				if(found)
					return removeKeyAt(i);
				else {
					return DEL_KEY_NOT_FOUND;
				}
			}
			break;
		default:
			return -1;
	}

	if(this->len <= minKeys){
		return this->growChildAndRemove(i, k, minKeys, typ);
	}

	Node *child = this->children[i];
	if(found){
		// 当找到时,children[i]是小于k的子树
		int out = this->keys[i];
		// 找到key[i]左子树的最大节点并删除，并将该值赋给key[i]
		// 由于原先的key[i]被覆盖掉
		// 这完成了：将key左子树的最大值转移到当前node，并将key删除的操作
		this->keys[i] = child->remove(-1, minKeys, removeMax);
		return out;
	}
	// 需要注意的是，当在当前node没找到key时，children[i]包括key，所以在children[i]删除key
	return child->remove(k, minKeys, typ);
}

int growChildAndRemove(int index, int key, int minKeys,  typ removeType){
	if(index > 0 && children[index-1].len > minKeys){
		// 左孩子节点足够
		Node *child = children[index];
		Node *stealFrom = children[index-1];
		int stolenKey = stealFrom.pop();
		child.insertKeyAt(0, keys[index-1]);
		keys[index-1] = stolenKey;
	}else if(index < len && children[index+1].len > minKeys){
		// 右孩子节点足够
	}else{
		// 都不够，合并

	}
	return 0;
}
// removeKeyAt removes the key of the given index
// Returns the removed key indicates remove successed
int Node::removeKeyAt(int index){
	if(index >= len){
		return -1;
	}
	int ret = keys[index];
	for(int i=len;i>=index+1;i--){
		keys[i] = keys[i-1];
	}
	len--;
	return ret;
}

// removeChildAt removes the child of the given key
// Returns the removed child indicates remove successed
// TODEL
Node* Node::removeChildAt(int index){
	if(index > len){
		return nullptr;
	}
	int ret = children[index];
	for(int i = len+1;i>=index+1;i--){
		children[i] = children[i-1];
	}
	return ret;
}

int Node::pop(){
	return removeKeyAt(len-1);
}