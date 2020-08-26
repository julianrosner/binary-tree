#pragma once
#include <iostream>		// std::cout, std::endl
#include <stack>		// std::stack
#include <utility>		// std::pair
#include <stdexcept>	// std::out_of_range

using std::pair;
using std::stack;
using std::ostream;

// TreeMap represents a map implemented as a binary search tree
// which supports deletion (but not the [] operator)
// and includes an input_iterator
// which performs lazy in-order iteration

// Usage Notes Concerning TreeMap and TreeIterator:

// 1. class K must support the <, >, and == operators

// 2. user retains responsibility for freeing keys and values
// if either K or V is a pointer type

// 3. if any key is altered after it is inserted into the
// tree, all behavior guarantees are immediately
// and permanently nullified

// 4. if the tree is modified after an iterator is constructed,
// said iterator is invalid and its behavior is not guaranteed.

template<class K, class V> class TreeMap {
	// struct representing a node in the tree
	typedef struct Node {
		pair<K, V> payload;
		Node* right;
		Node* left;
	} TreeMapNode;

	// a lazy input_iterator for TreeMap which performs an in-order
	// traversal of the tree in question
	class TreeIterator :
		public std::iterator<std::input_iterator_tag, pair<K, V>> {

	public:
		// constructs iterator of the subtree for which root is the root
		TreeIterator(TreeMapNode* root);

		// copy constructor
		TreeIterator(const TreeIterator& tit);

		// constructor for past-the-end iterator
		TreeIterator() : toBeProcessed_(new stack<TreeMapNode*>()) {};
		~TreeIterator() { delete toBeProcessed_; };

		// comparison operators.
		// two iterators are equal if they are at an identical
		// position in the same instance of a TreeMap
		// or if they are both past-the-end
		bool operator==(const TreeIterator& rhs) const;
		bool operator!=(const TreeIterator& rhs) const;

		// basic accessors, rvalues only
		// each throws out of range exception if
		// called when iterator is past-the-end
		const pair<K, V>& operator*() const;
		pair<K, V> const* operator->() const;

		// pre and postfix incrementers
		// each throws out of range exception if
		// called when iterator is past-the-end
		TreeIterator& operator++();
		TreeIterator operator++(int);

		// returns:
		// true iff iterator is in legal state to be dereferenced
		bool isLegal() const { return toBeProcessed_->size() != 0; };

	private:
		// working stack of node pointers
		stack<TreeMapNode*>* toBeProcessed_;
	};  // end class TreeIterator

public:
	// constructs empty TreeMap
	TreeMap() : size_(0), root_(nullptr) {};
	~TreeMap();

	// parameters:
	// key- represents the key in this pair 
	// and must implement the <, >, and == operators.
	// nullptr can be a valid key if K is pointer type
	// value- represents the value paired with key
	// can be nullptr if V is a pointer type
	// returns: 
	// true if there was enough space to allocate another node
	// AND this key is not equivalent to one in this tree already
	// else returns false
	// modifies:
	// map to contain given key-value pair if equal key is not present
	// if equivalent key is present, nothing is modified
	bool add(const K& key, const V& value);

	// parameters:
	// key- key of key-value pair which is to be retrieved
	// returns:
	// value corresponding to given key
	// throws:
	// out of range exception if no key in map is equivalent to given key
	V& at(const K& key) const;

	// parameters:
	// key- key of key-value pair which is to be removed
	// returns:
	// value corresponding to given key
	// modifies:
	// map to no longer contain key-value pair
	// throws:
	// out of range exception if no key in map is equal to given key
	V remove(const K& key);

	// returns:
	// number of key-value pairs in map
	unsigned int size() const;

	// returns:
	// iterator to beginning of tree, which performs in-order traversal
	TreeIterator begin() const { return TreeIterator(root_); }

	// returns:
	// past-the-end iterator for use in comparison
	TreeIterator end() const { return TreeIterator(); };

private:
	unsigned int size_;
	TreeMapNode* root_;

	// parameters:
	// current- root of tree which is to be deleted
	// modifies:
	// tree to not contain any nodes
	void deleteTreeHelper(TreeMapNode* current);

	// parameters:
	// current- root of subtree which is being added to
	// newElement- node which is being added
	// success- return parameter for whether the add succeeded
	// returns: 
	// root of newly modified tree
	// modifies:
	// map to contain newElement if its key isn't equivalent to one 
	// already in map
	TreeMap<K, V>::TreeMapNode* addHelper(TreeMapNode* current,
		TreeMapNode* newElement, bool* success);

	// parameters:
	// current- root of subtree which is being removed from
	// key- key of element which is to be removed
	// retVal- return parameter for value of element being removed
	// returns: 
	// root of newly modified tree
	// modifies:
	// removes element with matching key from map
	// throws:
	// out of range excpetion if no key match is found
	TreeMap<K, V>::TreeMapNode* removeHelper(TreeMapNode* current,
		const K& key, V* retVal);

	// parameters:
	// current- root of subtree in which lookup is desired
	// key- key of element which is to be looked up
	// returns: value associated with given key
	// throws:
	// out of range excpetion if no key match is found
	V& atHelper(TreeMapNode* current, const K& key) const;
};  // end class TreeMap

template<class K, class V>
TreeMap<K, V>::~TreeMap() {
	deleteTreeHelper(root_);
};

template<class K, class V>
void TreeMap<K, V>::deleteTreeHelper(TreeMapNode* current) {
	if (current != nullptr) {
		deleteTreeHelper(current->left);
		deleteTreeHelper(current->right);
		delete current;
	}
};

template<class K, class V>
bool TreeMap<K, V>::add(const K& key, const V& value) {
	// safely attempt to construct new node
	TreeMapNode* newElement;
	try {
		newElement = new TreeMapNode{ pair<K, V>(key, value), nullptr, nullptr };
	}
	catch (std::bad_alloc&) {
		return false;
	}

	bool success;
	root_ = addHelper(root_, newElement, &success);
	if (success) {  // only increment size if no key collision occured
		size_++;
	}
	return success;
};

// function takes a TMN instead of a key, value pair because that allows for
// nice re-use within removeHelper
template<class K, class V>
typename TreeMap<K, V>::TreeMapNode* TreeMap<K, V>::addHelper
(TreeMapNode* current, TreeMapNode* newElement, bool* success) {
	if (current == nullptr) {  // reached location where new element belongs
		*success = true;
		return newElement;
	}
	else if (current->payload.first < newElement->payload.first) {
		current->right = addHelper(current->right, newElement, success);
	}
	else if (current->payload.first > newElement->payload.first) {
		current->left = addHelper(current->left, newElement, success);
	}
	else {  // key collision, tree will not be altered
		*success = false;
		delete newElement;
	}
	return current;
};

template<class K, class V>
V TreeMap<K, V>::remove(const K& key) {
	V retVal;
	root_ = removeHelper(root_, key, &retVal);
	size_--;
	return retVal;
};

template<class K, class V>
typename TreeMap<K, V>::TreeMapNode*
TreeMap<K, V>::removeHelper(TreeMapNode* current, const K& key, V* retVal) {
	if (current == nullptr) {  // given key was bad
		throw std::out_of_range("No such key exists in this tree.");
	}
	else if (current->payload.first < key) {
		current->right = removeHelper(current->right, key, retVal);
	}
	else if (current->payload.first > key) {
		current->left = removeHelper(current->left, key, retVal);
	}
	else {
		*retVal = current->payload.second;
		// connect right subtree to left subtree in manner which
		// upholds BST properties
		// this can lead to some pretty poor tree balancing 
		// but this class makes no balance guarantees.

		// I experimented with a solution that added the left
		// and right subtrees individually to the root rather than stacking one
		// on to the other, and surprisingly saw no tree balance
		// improvements, so I reverted to this simpler solution.
		bool unused;
		TreeMapNode* remainingSubtree;
		if (current->right != nullptr) {  // we can't insert a null node into
			// tree so we first must check current->right is not null
			remainingSubtree = addHelper(current->left, current->right, &unused);
		}
		else {
			remainingSubtree = current->left;
		}
		// clean up removed node
		delete current;
		return remainingSubtree;
	}
	return current;
};

template<class K, class V>
V& TreeMap<K, V>::at(const K& key) const {
	return atHelper(root_, key);
}

template<class K, class V>
V& TreeMap<K, V>::atHelper(TreeMapNode* current, const K& key) const {
	if (current == nullptr) {
		throw std::out_of_range("No such key exists in this tree.");
	}
	else if (current->payload.first < key) {
		return atHelper(current->right, key);
	}
	else if (current->payload.first > key) {
		return atHelper(current->left, key);
	}
	else {
		return  current->payload.second;
	}
}

template<class K, class V>
unsigned int TreeMap<K, V>::size() const {
	return size_;
}

template<class K, class V>
TreeMap<K, V>::TreeIterator::TreeIterator(TreeMapNode* root)
	: toBeProcessed_(new stack<TreeMapNode*>()) {
	while (root != nullptr) {
		// to perform in-order traversal we must start
		// in the most left ancestor of the root
		// but remember those seen along the way
		toBeProcessed_->push(root);
		root = root->left;
	}
};

template<class K, class V>
TreeMap<K, V>::TreeIterator::TreeIterator(const TreeIterator& tit)
	: toBeProcessed_(new stack<TreeMapNode*>()) {
	*toBeProcessed_ = *tit.toBeProcessed_;
}

template<class K, class V>
bool TreeMap<K, V>::TreeIterator::operator==(const TreeIterator& rhs) const {
	return *toBeProcessed_ == *rhs.toBeProcessed_;
}

template<class K, class V>
bool TreeMap<K, V>::TreeIterator::operator!=(const TreeIterator& rhs) const {
	return *toBeProcessed_ != *rhs.toBeProcessed_;
}

template<class K, class V>
typename TreeMap<K, V>::TreeIterator&
TreeMap<K, V>::TreeIterator::operator++() {
	if (!isLegal()) {
		throw std::out_of_range("This iterator is past its end.");
	}
	TreeMapNode* current = toBeProcessed_->top();
	toBeProcessed_->pop();
	// if possible move to right child once
	// then move left as many times as possible
	if (current->right != nullptr) {
		current = current->right;
		while (current != nullptr) {
			toBeProcessed_->push(current);
			current = current->left;
		}
	}
	return *this;
}

template<class K, class V>
typename TreeMap<K, V>::TreeIterator
TreeMap<K, V>::TreeIterator::operator++(int) {
	TreeIterator tmp(*this);
	operator++();
	return tmp;
}

template<class K, class V>
const pair<K, V>& TreeMap<K, V>::TreeIterator::operator*() const {
	if (!isLegal()) {
		throw std::out_of_range("This iterator is past its end.");
	}
	return toBeProcessed_->top()->payload;
}

template<class K, class V>
pair<K, V> const* TreeMap<K, V>::TreeIterator::operator->() const {
	if (!isLegal()) {
		throw std::out_of_range("This iterator is past its end.");
	}
	TreeMapNode* current = toBeProcessed_->top();
	return &(current->payload);
}

// writes in-order traversal of tm's nodes to given ostream
template<class K, class V>
ostream& operator<<(ostream& os, const TreeMap<K, V>& tm) {
	auto it = tm.begin();
	if (tm.size() > 0) {
		for (unsigned int i = 0; i < tm.size() - 1; i++) {
			os << "{" << it->first << "=" << it->second << "}, ";
			++it;
		}
		os << "{" << it->first << "=" << it->second << "}";
	}
	return os;
}