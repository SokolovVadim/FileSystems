#include "btree.hpp"
#include <iostream>

BTree::BTree(int degree):
	degree_(degree),
	root_(nullptr)
{}

void BTree::insert(int64_t key)
{
	if(root_ == nullptr)
	{
		root_ = new Node(degree_);
		root_->insert(key);
		std::cout << "inserted to the root!\n";
	}
	else
	{
		// search for key in the tree
	}

}

Node* BTree::search(int64_t key)
{
	if(root_ == nullptr)
		return nullptr;
	else
		return root_->search(key);
}
