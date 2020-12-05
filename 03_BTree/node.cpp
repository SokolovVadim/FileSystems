#include "node.hpp"
#include <iostream>

Pair::Pair():
	key_(0),
	child_(nullptr)
{}

Node::Node(int degree):
	degree_(degree),
	keyNum_(0),
	isDeleted_(false),
	isLeaf_(false),
	data_(std::vector<Pair>(2 * degree))
{
	std::cout << "Node constructed!\ndegree = " << degree_ << std::endl;
}

void Node::shift(int64_t key, int64_t idx)
{
	for(int i(idx + 2); i < int64_t(keyNum_ + 1); ++i)
	{
		data_[i] = data_[i - 1];
	}
	data_[idx + 1].key_ = key;
}

void Node::insert(int64_t key)
{
	if(keyNum_ == 0)
	{
		data_[keyNum_].key_ = key;
		keyNum_++;
		std::cout << "Key " << key << " inserted!\n";
	}
	if((keyNum_ > 0) && (keyNum_ < degree_ * 2 - 1)){
		for(int64_t i(keyNum_ - 1); i >= 0; --i)
		{
			if(key > data_[i].key_)
			{
				shift(key, i);
				std::cout << "Shift case! Key " << key << " inserted!\n";
			}
		}
		
	}
	else // keyNum_ == degree_ * 2 - 1, node fullfilled
	{
		split(data_[keyNum_].child_, keyNum_, key);
	}
}

/*
5 8 12 6

	5 8 12
	
		8
	5 6		12
*/

void Node::split(Node* child, int idx, int64_t key)
{
	Node* newNode = new Node(degree_);
	newNode->insert(key);
}

Node* Node::search(int64_t key)
{
	for(int64_t i(0); i < keyNum_; ++i)
	{
		if(key == data_[i].key_)
		{
			std::cout << "key " << key << " found!\n";
			return this;
		}
	}

	// if leaf -> return nullptr;
	// return data_[keyNum_ - 1].child_->search(key);
	return nullptr;
}