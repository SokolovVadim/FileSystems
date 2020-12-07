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

void Node::shift_right(int64_t key, int64_t idx)
{
	std::cout << "idx: " << idx << std::endl;
	for (int64_t i = keyNum_; i > idx + 1; --i)
	{
		data_[i] = data_[i - 1];
	}
	data_[idx + 1].key_ = key;
	keyNum_++;
}

void Node::shift_left(int64_t key)
{
	std::cout << "key num: " << keyNum_ << std::endl;
	for (int64_t i = keyNum_; i > 0; --i)
	{
		data_[i] = data_[i - 1];
	}
	data_[0].key_ = key;
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

	Node* newNode = new Node(degree_, true);
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
	this->isLeaf_ = true;

	// set new node as a right child
	newRoot->data_[1].child_ = newNode;

	// insert key
	if (key > newRoot->data_[0].key_)
	{
		newRoot->data_[1].child_->insert(key);
	}
	else
	{
		newRoot->data_[0].child_->insert(key);
	}

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
	if (this == nullptr)
		return;
	if (isLeaf_ == true) {
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
					shift_right(key, i);
					std::cout << "Shift right case! Key " << key << " inserted!\n";
					break;
				}
			}
			if (key < data_[0].key_) {
				std::cout << "Shift left case! Key " << key << " inserted!\n";
				shift_left(key);
			}
		}
	}
	else
	{
		std::cout << "I'm not leaf!\n";
		int64_t i = keyNum_ - 1;
		while (i >= 0 && data_[i].key_ > key) {
			i--;
		}
		std::cout << "i: " << i << std::endl;
		// node fulfilled
		if (data_[i + 1].child_ && data_[i + 1].child_->keyNum_ == 2 * degree_ - 1) {
			split(data_[i + 1].child_, i + 1);

			if (data_[i + 1].key_ < key) {
				i++;
			}
		}
		data_[i + 1].child_->insert(key);
	}
}

/*
5 8 12 6 22

	5 8 12

		8
	5 6		12 22
*/

void Node::split(Node* node, int idx)
{
	Node* newNode = new Node(node->degree_, node->isLeaf_);
	newNode->keyNum_ = degree_ - 1;

	for (int i(0); i < degree_ - 1; ++i) {
		newNode->data_[i] = node->data_[i + degree_];
	}

	node->keyNum_ = degree_ - 1;

	for (int i(keyNum_); i >= idx; --i)
	{
		data_[i + 1] = data_[i];
	}
	this->data_[idx + 1].child_ = newNode;
	this->data_[idx].key_ = node->data_[degree_ - 1].key_;
	keyNum_++;
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