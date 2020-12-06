#include "node.hpp"
#include <iostream>

Pair::Pair() :
	key_(0),
	child_(nullptr)
{}

Node::Node(int degree, bool isLeaf) :
	degree_(degree),
	keyNum_(0),
	isDeleted_(false),
	isLeaf_(isLeaf),
	data_(std::vector<Pair>(2 * degree))
{
	std::cout << "Node constructed!\ndegree = " << degree_ << std::endl;
}

void Node::shift(int64_t key, int64_t idx)
{
	for (int64_t i(idx + 2); i < keyNum_ + 1; ++i)
	{
		data_[i] = data_[i - 1];
	}
	data_[idx + 1].key_ = key;
	keyNum_++;
}

const int64_t Node::getKeyNum() const
{
	return this->keyNum_;
}

bool Node::isNodeFull()
{
	if (keyNum_ == degree_ * 2 - 1)
		return true;
	else
		return false;
}

// root is fulfilled
Node* Node::splitRoot(int64_t key)
{
	// create new root with middle elem
	Node* newRoot = new Node(degree_, false);
	newRoot->data_[0].child_ = this;
	newRoot->data_[0].key_ = this->data_[degree_ - 1].key_;
	newRoot->keyNum_ = 1;

	Node* newNode = new Node(degree_, false);
	// copy contents of root to the newNode
	for (int i(0); i < degree_ - 1; ++i)
	{
		newNode->data_[i] = data_[i + degree_];
	}
	newNode->keyNum_ = degree_ - 1;
	newNode->printNode();

	// clear old root's right data

	for (int i(0); i < degree_ - 1; ++i)
	{
		this->data_[i + degree_ - 1].child_ = nullptr;
		this->data_[i + degree_ - 1].key_ = 0;
	}
	this->keyNum_ = degree_ - 1;

	// set new node as a right child
	newRoot->data_[1].child_ = newNode;
	newRoot->printNode();
	return newRoot;
}

void Node::printNode()
{
	std::cout << "--------- PRINT_NODE START ---------" << std::endl;
	std::cout << "NODE " << this << std::endl;
	std::cout << "degree: " << degree_ << std::endl;
	for (int i(0); i < keyNum_; ++i)
	{
		std::cout << "key [" << i << "] = " << data_[i].key_ << "\t child[" << i << "] = " << data_[i].child_ <<  std::endl;
	}
	std::cout << "--------- PRINT_NODE  END  ---------" << std::endl;
}

void Node::printAll()
{
	printNode();
	for (int64_t i(0); i < keyNum_ + 1; ++i)
	{
		if (data_[i].child_ != nullptr)
		{
			data_[i].child_->printAll();
		}
	}
}



void Node::insert(int64_t key)
{
	if (keyNum_ == 0)
	{
		data_[keyNum_].key_ = key;
		keyNum_++;
		std::cout << "Key " << key << " inserted!\n";
	}
	if ((keyNum_ > 0) && (keyNum_ < degree_ * 2 - 1)) {
		for (int64_t i(keyNum_ - 1); i >= 0; --i)
		{
			if (key > data_[i].key_)
			{
				shift(key, i);
				std::cout << "Shift case! Key " << key << " inserted!\n";
				break;
			}
		}

	}
	else // keyNum_ == degree_ * 2 - 1, node fullfilled
	{
		std::cout << "Split case!\n";
		split(data_[keyNum_].child_, keyNum_, key);
	}
}

/*
5 8 12 6

	5 8 12

		8
	5 6		12
*/

void Node::split(Node* node, int idx, int64_t key)
{
	Node* newNode = new Node(node->degree_, node->isLeaf_);
}

Node* Node::search(int64_t key)
{
	if (this == nullptr)
		return nullptr;
	for (int64_t i(0); i < keyNum_; ++i)
	{
		if (key == data_[i].key_)
		{
			std::cout << "key " << key << " found!\n";
			return this;
		}
	}
	if (isLeaf_)
		return nullptr;
	return data_[keyNum_ - 1].child_->search(key);
}