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
	tree.insert(8);
	tree.insert(13);
	tree.insert(5);
	tree.insert(0);
	tree.insert(16);
	tree.insert(7);
	tree.insert(23);
	tree.insert(48);
	tree.insert(15);
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);
	tree.insert(4);

	std::cout << "---------------------------\n";
	tree.printTree();
	tree.printDot();

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

