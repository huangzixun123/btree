#include<iostream>
#include "btree.h"
using namespace std;

int main()
{
	// degree = 3  == order = 5
	// order = 2 * degree - 1
	BTree t(3);
	t.ReplaceOrInsert(10);
	t.ReplaceOrInsert(20);
	t.ReplaceOrInsert(5);
	t.ReplaceOrInsert(6);
	t.ReplaceOrInsert(12);
	t.ReplaceOrInsert(30);
	t.ReplaceOrInsert(7); 
	t.ReplaceOrInsert(17);
	t.ReplaceOrInsert(40);
	t.ReplaceOrInsert(50);
	cout << "Traversal of the constructed tree is "<<endl;
	t.Traverse();

	int k = 6;
	(t.Search(k) != nullptr)? cout << "\nPresent1" : cout << "\nNot Present1";
	k = 15;
	(t.Search(k) != nullptr)? cout << "\nPresent2" : cout << "\nNot Present2";
	
	return 0;
}