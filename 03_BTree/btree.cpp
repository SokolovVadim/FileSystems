#include "btree.hpp"


BTree::BTree(int degree):
	degree_(degree),
	root_(nullptr)
{}

void BTree::insert(int key)
{
	if(root_ == nullptr)
	{
		root_ = new Node(degree_);
		root_->insert(key);
	}
}