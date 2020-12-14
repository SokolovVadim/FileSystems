#include "btree.hpp"
#include <iostream>
#include <fstream>

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
	if (root_ == nullptr)
	{
		std::cout << "Root is empty. Insert elements\n";
		return;
	}
	std::cout << "------------------------------------\n";
	std::cout << "--------- PRINT TREE START ---------\n";
	root_->printAll();
	std::cout << "---------- PRINT TREE END ----------\n";
	std::cout << "------------------------------------\n";
}

void BTree::printDot()
{
	std::string msg{};
	if (root_ == nullptr)
	{
		msg += "Tree is empty!\n";
	}
	else
	{
		msg = "digraph Tree {\n";
		root_->printDot(msg);
		msg += "\n}";
		write_to_file(msg);
	}
}

void BTree::write_to_file(const std::string& msg)
{
	std::ofstream fout("out.dot");
	if (fout.is_open())
	{
		fout.write(msg.c_str(), msg.length());
	}
	fout.close();
}
