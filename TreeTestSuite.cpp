#include "TreeMap.h"	// TreeMap, TreeIterator

#include <utility>		// std::pair
#include <stdexcept>    // std::out_of_range
#include <iostream>     // std::cout, std::endl
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <cassert>		// assert

using std::cout;
using std::endl;
using std::vector;
using std::pair;

int main(int argv, char** argc) {
	cout << "PLEASE ENSURE THAT ASSERT STATEMENTS ARE ENABLED." << endl;
	cout << "IF THEY AREN'T, NOT MUCH WILL BE TESTED HERE." << endl << endl;
	cout << "COMMENCING TESTING..." << endl << endl;
	cout << "COMMENCING SIZE 1 MAP TESTS..." << endl;

	// construct empty bst map
	TreeMap<int, char> bst1 = TreeMap<int, char>();
	assert(bst1.size() == 0);

	// test that insert and look-up behave
	// as expected on initially size 0 / 1 tree
	assert(bst1.add(0, 'a'));
	assert(bst1.at(0) == 'a');
	assert(bst1.size() == 1);

	// test that inserting on previously seen key fails
	// and doesn't alter value associated with key
	assert(!bst1.add(0, 'b'));
	assert(bst1.at(0) == 'a');
	assert(bst1.size() == 1);

	// test remove works on size 1 TreeMap
	assert(bst1.remove(0) == 'a');
	assert(bst1.size() == 0);

	// test that once removed, old key doesn't
	// prevent equivalent key from being added
	assert(bst1.add(0, 'b'));
	assert(bst1.at(0) == 'b');
	assert(bst1.size() == 1);

	cout << "SIZE 1 MAP TESTS: COMPLETE" << endl << endl;

	cout << "COMMENCING EXCEPTION TESTS..." << endl;

	TreeMap<char, bool> bst2 = TreeMap<char, bool>();

	try {
		bst2.at('a');
		assert(false);
	}
	catch (std::out_of_range) {

	}

	try {
		bst2.remove('a');
		assert(false);
	}
	catch (std::out_of_range) {

	}

	cout << "EXCEPTION TESTS: COMPLETE" << endl << endl;

	cout << "COMMENCING BULK ADD TEST..." << endl;
	const int BULK_SIZE = 1000;
	TreeMap<int, int> bst3 = TreeMap<int, int>();

	for (int i = 0; i < BULK_SIZE; i++) {
		assert(bst3.add(i, -i));
	}
	assert(bst3.size() == BULK_SIZE);

	cout << "BULK ADD TEST: COMPLETE" << endl << endl;

	cout << "COMMENCING BULK AT TEST..." << endl;

	for (int i = 0; i < BULK_SIZE; i++) {
		// check that lookup value are as expected
		assert(bst3.at(i) == -i);

		// check that at()'s return functions as an lvalue
		bst3.at(i) = -1 * bst3.at(i);
		assert(bst3.at(i) == i);

		// revert elements to as they were and verify success
		bst3.at(i) = -1 * bst3.at(i);
		assert(bst3.at(i) == -i);
	}
	assert(bst3.size() == BULK_SIZE);

	cout << "BULK AT TEST: COMPLETE" << endl << endl;

	cout << "COMMENCING BULK REMOVE TEST..." << endl;

	for (int i = 0; i < BULK_SIZE; i++) {
		assert(bst3.remove(i) == -i);
	}
	assert(bst3.size() == 0);

	cout << "BULK REMOVE TEST: COMPLETE" << endl << endl;

	cout << "COMMENCING REMOVE NODE ALL CASES..." << endl;
	// there are four major cases for a removed node's children
	// 1. two children nodes present
	// 2. left child but no right child
	// 3. right child but no left child
	// 4. no children

	TreeMap<int, char> bst4 = TreeMap<int, char>();
	// case 1
	assert(bst4.add(0, 'a'));  // parent
	assert(bst4.add(-1, 'b'));  // left child
	assert(bst4.add(1, 'c'));  // right child

	// remove parent and check state
	assert(bst4.remove(0) == 'a');
	assert(bst4.at(-1) == 'b');
	assert(bst4.at(1) == 'c');
	assert(bst4.size() == 2);

	// clean up
	assert(bst4.remove(-1) == 'b');
	assert(bst4.remove(1) == 'c');

	// case 2
	assert(bst4.add(0, 'a'));  // parent
	assert(bst4.add(-1, 'b'));  // left child

	// remove parent and check state
	assert(bst4.remove(0) == 'a');
	assert(bst4.at(-1) == 'b');
	assert(bst4.size() == 1);

	// clean up
	assert(bst4.remove(-1) == 'b');

	// case 3
	assert(bst4.add(0, 'a'));  // parent
	assert(bst4.add(1, 'c'));  // right child

	// remove parent and check state
	assert(bst4.remove(0) == 'a');
	assert(bst4.at(1) == 'c');
	assert(bst4.size() == 1);

	// clean up
	assert(bst4.remove(1) == 'c');

	// case 4
	assert(bst4.add(0, 'a'));  // parent

	// remove parent and check state
	assert(bst4.remove(0) == 'a');
	assert(bst4.size() == 0);

	cout << "REMOVE NODE ALL CASES: COMPLETE" << endl << endl;

	cout << "COMMENCING ITERATOR TESTS..." << endl;
	TreeMap<int, char> bst5 = TreeMap<int, char>();
	assert(bst5.add(1, 'a'));
	assert(bst5.add(2, 'b'));
	assert(bst5.add(3, 'c'));
	auto it1 = bst5.begin();

	// test dereference operator
	pair<int, char> cur = *it1;
	assert(1 == cur.first);
	assert('a' == cur.second);
	assert(it1->first == 1);

	// test class member access operator
	assert(it1->first == 1);
	assert(it1->second == 'a');

	// test prefix ++ operator
	assert((++it1)->first == 2);
	assert(it1->first == 2);
	assert(it1->second == 'b');

	// test postfix ++ operator
	assert((it1++)->first == 2);
	assert(it1->first == 3);
	assert(it1->second == 'c');

	// test equality, inequality, and isLegal()
	auto it2 = bst5.begin();
	assert(it2.isLegal());
	assert(it1 != it2);

	++it2;
	assert(it2.isLegal());
	assert(it1 != it2);

	++it2;
	assert(it2.isLegal());
	assert(it1 == it2);

	++it2;
	assert(!it2.isLegal());
	assert(it1 != it2);

	// test past-the-end iterator equality
	assert(it1 != bst5.end());
	assert(it2 == bst5.end());

	// check correct exceptions are thrown
	try {
		++it2;
		assert(false);
	}
	catch (std::out_of_range) {

	}

	try {
		auto otherIt = it2++;
		assert(false);
	}
	catch (std::out_of_range) {

	}

	try {
		*it2;
		assert(false);
	}
	catch (std::out_of_range) {

	}

	try {
		it2->first;
		assert(false);
	}
	catch (std::out_of_range) {

	}
	cout << "ITERATOR TESTS: COMPLETE" << endl << endl;

	cout << "COMMENCING RANDOMIZED TREE STRESS TESTS..." << endl;
	const int NUM_RANDOMLY_ORDERED_ELEMENTS = 50000;
	TreeMap<int, int> bst6 = TreeMap<int, int>();
	vector<int> ints;
	// push values in range [0, NUM_RANDOMLY_ORDERED_ELEMENTS)
	// into vector in ascending order, then shuffle them
	for (int i = 0; i < NUM_RANDOMLY_ORDERED_ELEMENTS; i++) {
		ints.push_back(i);
	}
	std::random_shuffle(ints.begin(), ints.end());

	// add content of shuffled vector into tree 
	// paired with a value which tracks the order
	// in which each element was inserted
	int distance = 0;
	for (auto rit = ints.begin(); rit != ints.end(); rit++) {
		assert(bst6.add(*rit, distance));
		distance++;
	}

	assert(bst6.size() == NUM_RANDOMLY_ORDERED_ELEMENTS);

	// check that every key-value pair is still correct
	distance = 0;
	for (auto rit = ints.begin(); rit != ints.end(); rit++) {
		assert(bst6.at(*rit) == distance);
		distance++;
	}

	// shuffle the vector again and remove all elements
	// in a newly randomized order independent of the 
	// order in which the elements were added
	std::random_shuffle(ints.begin(), ints.end());
	for (auto rit = ints.begin(); rit != ints.end(); rit++) {
		bst6.remove(*rit);
	}

	assert(bst6.size() == 0);
	cout << "RANDOMIZED TREE STRESS TESTS: COMPLETE" << endl << endl;

	cout << "ALL TESTS SUCCESSFULLY COMPLETED" << endl;
	return EXIT_SUCCESS;
}