/*
The Class Node is the each node in the tree.
Information stored in Node:
'Feature' that has been chosen at this node,
'Split point' that split the examples in this node into left branch and right branch.
'sub nodes' of this node, e.g. the left sub-node and the right sub-node.
if the node is a leaf, then it will have 'label'
*/
#ifndef NODE_H
#define NODE_H

#include "common.h"

class Node {
public:
    /* 
        Constructor for non-leave nodes.
    */
	Node(feature a, int d);
    /*
        Constructor for leave nodes. 
    */
	Node(example averageLabelValue, int d);

    Node(vector<example> examples, int d);
    /*
        Add a new branch to this node, and the split poiont value also needed.
        We at most add two branch to the node.
    */
	void addNewBranch(Node* s, float sP);
    /*
        When given a test example, let the Node make a decision to left or right sub-node.
    */
	example makeDecision(example &e); 
    /* 
        Print out the the value of this node.
        For debug only.
    */
	void print();
    /*
        If this node is a leaf noed, return true.
    */
	bool isLeave();
	vector<Node*> getSubNodes();
	float getSplitPoint(); 
    /*
        Deconstructor. will recursively delete its sub-nodes.
    */
	~Node();
private:
    vector<example> estimateExamples;
	feature chosenFeature; // leaves don't have this value.
	example label; // only leaves have this value.
	vector<Node*> subNodes; // only two sub Nodes here. 
	float splitPoint; // leftNode < splitPoint, RightNode > splitPoint.
	int depth; // the depth of this node.
};

#endif
