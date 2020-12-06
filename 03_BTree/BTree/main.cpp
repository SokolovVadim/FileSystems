#include <iostream>
#include "node.hpp"
#include "btree.hpp"

bool test_node();
bool test_tree();

int main()
{
	std::cout << "Hello!\n";
	// test_node();
	std::cout << "---------------------------\n";
	test_tree();
}

bool test_tree()
{
	BTree tree(2);
	tree.insert(5);
	tree.insert(8);

	tree.search(5);

	tree.insert(9);
	tree.insert(10);

	return true;
}

bool test_node()
{
	Node node(5, false);
	for (int i(0); i < 10; ++i)
	{
		node.insert(i);
	}
	node.search(1);
	return true;
}

