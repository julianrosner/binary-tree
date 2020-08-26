# bst-map
BST Map Written in c++ Along with a Lazy In-Order Traversing Iterator

by Julian Rosner

The map supports deletion, and the the iterator supports any operation
you would expect from an input_iterator. Currently the map perfroms no
self-balancing techniques, although I have been thinking about coming back
and implementing this.

The way that I've partitioned code into files may seem unusual at first. 
The ideal arrangement would probably be to have my bst source code in four 
files called BSTMap.cc, BSTMap.h, BSTIterator.cc, and BSTIterator.h, but 
c++'s generic typing quirks make that impratical, so I've chosen to
simply leave the code together in one file because that feels like
the least hack-y solution.
