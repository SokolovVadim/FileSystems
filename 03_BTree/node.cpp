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
	/*std::cout << "idx: " << idx << std::endl;*/
	for (int64_t i = keyNum_; i > idx + 1; --i)
	{
		data_[i] = data_[i - 1];
	}
	data_[idx + 1].key_ = key;
	keyNum_++;
}

void Node::shift_left(int64_t key)
{
	/*std::cout << "key num: " << keyNum_ << std::endl;*/
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

const bool Node::isLeaf() const
{
	return isLeaf_;
}

// root is fulfilled
Node* Node::splitRoot(int64_t key)
{
	// create new root with middle elem
	Node* newRoot = new Node(degree_, false);
	newRoot->data_[0].child_ = this;

	newRoot->splitChild(0, this);
	int i(0);
	if (newRoot->data_[0].key_ < key)
		i++;
	newRoot->data_[i].child_->insert(key);

	return newRoot;
}

void Node::printNode()
{
	if (this == nullptr)
		return;
	std::cout << "--------- PRINT_NODE START ---------" << std::endl;
	std::cout << "NODE " << this << std::endl;
	std::cout << "degree: " << degree_ << std::endl;
	std::cout << "isLeaf: " << isLeaf_ << std::endl;
	for (int i(0); i < keyNum_; ++i)
	{
		std::cout << "key [" << i << "] = " << data_[i].key_ << "\t child[" << i << "] = " << data_[i].child_ <<  std::endl;
	}
	// std::cout << "\t\t child[" << keyNum_ << "] = " << data_[keyNum_].child_ << std::endl;
	std::cout << "--------- PRINT_NODE  END  ---------" << std::endl;
}

void Node::printAll()
{
	if (this == nullptr)
		return;
	printNode();
	for (int i(0); i < keyNum_ + 1; ++i)
	{
		if (data_[i].child_ != nullptr)
		{
			data_[i].child_->printAll();
		}
	}
}

void Node::splitChild(int idx, Node* child)
{
	// create new node
	Node* newNode = new Node(child->degree_, child->isLeaf_);
	newNode->keyNum_ = degree_ - 1;

	// copy data from this to new node
	for (int i(0); i < degree_ - 1; ++i) {
		newNode->data_[i].key_ = child->data_[i + degree_].key_;
	}

	// copy children
	if (child->isLeaf_ == false)
	{
		for (int i(0); i < degree_; ++i)
			newNode->data_[i].child_ = child->data_[i + degree_].child_;
	}

	// Reduce keyNum in child
	child->keyNum_ = degree_ - 1;

	for (int i = keyNum_; i >= idx + 1; --i)
		data_[i + 1].child_ = data_[i].child_;

	// Add new child

	data_[idx + 1].child_ = newNode;

	for (int i(keyNum_); i >= idx; --i)
		data_[i + 1].key_ = data_[i].key_;

	// Copy middle elem to this

	data_[idx].key_ = child->data_[degree_ - 1].key_;
	keyNum_++;
}

void Node::insert(int64_t key)
{
	int i = keyNum_ - 1;
	if (isLeaf_ == true)
	{
		// shift
		while (i >= 0 && data_[i].key_ > key)
		{
			data_[i + 1].key_ = data_[i].key_;
			i--;
		}
		// insert

		data_[i + 1].key_ = key;
		keyNum_++;
	}
	else // not leaf
	{
		while (i >= 0 && data_[i].key_ > key)
			i--;
		if (data_[i + 1].child_->isNodeFull())
		{
			splitChild(i + 1, data_[i + 1].child_);

			if (data_[i + 1].key_ < key)
				i++;
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

Node* Node::search(int64_t key)
{
	std::cout << "Searching for " << key << " ..." << std::endl;
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
	int64_t idx(0);
	for (int64_t i(0); i < keyNum_; ++i)
	{
		if (key < data_[i].key_)
		{
			idx = i;
		}
	}
	return data_[idx].child_->search(key);
}

void Node::printDot(std::string& msg)
{
	if (this == nullptr)
		return;
	if (this)
	{
		msg += "\"" + std::to_string((int64_t)this) + "\" [label = \"";
		for (int i(0); i < keyNum_ - 1; ++i)
		{
			msg += std::to_string(data_[i].key_) + " | ";
		}
		msg += std::to_string(data_[keyNum_ - 1].key_) + "\"];\n";
	}
	for (int i(0); i < keyNum_ + 1; ++i)
	{
		if ((this) && (data_[i].child_))
		{
			msg += "\"" + std::to_string((int64_t)this) + "\"->\"";
			msg += std::to_string(int64_t(data_[i].child_)) + "\";\n";
		}
	}
	for (int i(0); i < keyNum_ + 1; ++i)
	{
		if (data_[i].child_)
			data_[i].child_->printDot(msg);
	}
}