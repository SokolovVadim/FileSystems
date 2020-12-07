#include "btree.hpp"
#include <iostream>

BTree::BTree(int degree) :
	degree_(degree),
	root_(nullptr)
{}

void BTree::insert(int64_t key)
{
	if (root_ == nullptr)
	{
		root_ = new Node(degree_, true);
		root_->insert(key);
		std::cout << "inserted to the root!\n";
	}
	else
	{
		std::cout << "key num: " << root_->getKeyNum() << std::endl;
		// search for key in the tree
		if (search(key) == nullptr)
		{
			std::cout << "Key " << key << " does not exist! Inserting ...\n";
			if (root_->isNodeFull())
			{
				root_->printNode();
				std::cout << "Split root case\n";
				root_ = root_->splitRoot(key);
			}
			else
				root_->insert(key);
		}
	}
}

Node* BTree::search(int64_t key)
{
	if (root_ == nullptr)
		return nullptr;
	else
		return root_->search(key);
}

void BTree::printTree()
{
	std::cout << "------------------------------------\n";
	std::cout << "--------- PRINT TREE START ---------\n";
	root_->printAll();
	std::cout << "---------- PRINT TREE END ----------\n";
	std::cout << "------------------------------------\n";
}
