#include <iostream>
#include "node.hpp"
#include "btree.hpp"

bool test_node();
bool test_tree();

int main()
{
	std::cout << "Hello!\n";
	test_node();
	std::cout << "---------------------------\n";
	test_tree();
}

bool test_tree()
{
	BTree tree(5);
	tree.insert(10);
	tree.insert(11);
	tree.search(10);
	return true;
}

bool test_node()
{
	Node node(5);
	for(int i(0); i < 10; ++i)
	{
		node.insert(i);
	}
	node.search(1);
	return true;
}

