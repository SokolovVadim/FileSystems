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

void Node::insert(int64_t key)
{
	if(keyNum_ == 0)
	{
		data_[keyNum_].key_ = key;
		keyNum_++;
		std::cout << "Key " << key << " inserted!\n";
	}
	if((keyNum_ > 0) && (keyNum_ < degree_ * 2)){
		if(key > data_[keyNum_ - 1].key_)
		{
			data_[keyNum_].key_ = key;
			keyNum_++;
			std::cout << "Key " << key << " inserted!\n";
		}
		else
		{
			// split
		}
	}
}